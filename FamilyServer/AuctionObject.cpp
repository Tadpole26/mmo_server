
#include "AuctionObject.h"
#include "AuctionGoods.h"
#include "AuctionMenu.h"
#include "FamilyUser.h"
#include "AuctionEntity.h"
#include "AucitonBid.h"

AuctionObject::AuctionObject(cAuction::ObjectIdType id) :
	_id(id)
{
}

AuctionObject::~AuctionObject()
{
	SAFE_DELETE(_object);
	//SAFE_DELETE(_owner);
}

uint64 AuctionObject::getObjectNum() const
{
	if (!objectPtr()) return 0;
	//获取道具数量 TODO
}

bool AuctionObject::init()
{
	if (_biddingIndex.empty())
	{
		return true;
	}
	_biddingIndex.sort([](const auto* lval, const auto* rval) 
		{
			return lval->getBidId() < rval->getBidId();
		});
	_leadingBid = _biddingIndex.front();
	_biddingIndex.pop_front();
	return true;
}

void AuctionObject::tick()
{
	if (_biddingIndex.empty()) return;
	bool updated = false;
	auto curTime = GetCurrTime();
	if (curTime + 60 >= getBidExpire())
	{
		if (goodsPtr())
		{
			goodsPtr()->removeExpire(*this);
			if (auto* menu = goodsPtr()->menuPtr())
			{
				menu->removeObject(*this);
			}
			setDelayTimes(getDelayTimes() + 1);
			setBidExpire(getBidExpire() + 300);
			goodsPtr()->addExpire(*this);
			if (auto* menu = goodsPtr()->menuPtr())
			{
				menu->addObject(*this);
			}
			updated = true;
		}
	}

	auto nextPrice = getNextPrice();
	auto* leadingBid = _leadingBid;
	if (leadingBid)
	{
		_biddingIndex.push_front(leadingBid);
	}
	std::list<AuctionBid*> beaten;
	for (auto it = _biddingIndex.begin(); it != _biddingIndex.end())
	{
		auto del = it++;
		auto* bid = *del;

		if (bid->getPriceNum() < nextPrice)
		{
			_biddingIndex.earse(del);
			beaten.push_back(bid);
			continue;
		}
		bid->evAuctionBidPriceUpdated(getPrice(), nextPrice);
	}
	if (_biddingIndex.empty())
	{
		_leadingBid = beaten.front();
		beaten.pop_front();
		assert(_leadingBid == leadingBid);
	}
	else
	{
		_leadingBid = _biddingIndex.front();
		_biddingIndex.pop_front();
		auto formerPrice = getPrice();
		bool priceRankChanged = getPrice() ? cAuction::getPriceRank(getPrice()) != cAuction::getPriceRank(nextProce) : false;
		if (priceRankChanged && goodsPtr() && goodsPtr()->menuPtr())
		{
			goodsPtr()->menuPtr()->removeObject(*this);
		}
		updated = true;
		if (leadingBid != _leadingBid)
		{
			_leadingBid->evAuctionBidPriceLeading();
			if (leadingBid)
			{
				setBeatenSnapshot(leadingBid->snapshotPtr());
			}
		}
	}
	for (auto* bid : beaten)
	{
		bid->evAuctionBidPriceBeaten();
	}
	if (updated && entityPtr())
	{
		entityPtr()->objectUpdated(*this);
	}
}

void AuctionObject::evAuctionObjectExpired()
{
	__clearRelations();
}

void AuctionObject::evAuctionObjectDown()
{
	__clearRelations();
}

bool AuctionObject::addBid(AuctionBid& bid)
{
	auto ret = _bids.emplace(bid.getRoleId(), &bid);
}

bool AuctionObject::removeBid(AuctionBid& bid)
{
	_bids.erase(bid.getRoleId());
}

AuctionBid* AuctionObject::getBid(zRoleIdType userId)
{
	auto it = _bids.find(userId);
	return it == _bids.end() ? nullptr : it->second;
}

bool AuctionObject::foreachBid(BidVisitFuncT&& func) const
{
	return std::all_of(_bids.begin(), _bids.end(), [&](auto& pair) { return func(*pair.second); });
}

void AuctionObject::addBidding(AuctionBid& bid)
{
	_biddingIndex.push_back(&bid);
}

bool AuctionObject::addSubscription(AuctionSubscription& subscription)
{
	auto ret = _subscriptions.emplace(subscription.getRoleId(), &subscription);
	if (!ret.second) return false;
	return true;
}

void AuctionObject::removeSubscription(AuctionSubscription& subscription)
{
	_subscriptions.erase(subscription.getRoleId());
}

AuctionSubscription* AuctionObject::getSubscription(zRoleIdType userId)
{
	auto it = _subscriptions.find(userId);
	return it == _subscriptions.end() ? nullptr : it->second;
}

bool AuctionObject::foreachSubscription(SubscriptionVisitFuncT&& func) const
{
	return std::all_of(_subscriptions.begin(), _subscriptions.end(), [&](auto& pair) { return func(*pair.second); });
}

void AuctionObject::initWaitingExpire()
{
	assert(getGoodsUpTime());
	auto suspendBegin = getSuspendTimeBegin();
	if (!suspendBegin)
	{
		setWaitingExpire(getGoodsUpTime());
		return;
	}
	auto suspendEnd = getSuspendTimeEnd();
	if (!suspendEnd)
	{
		assert(false);
		setWaitingExpire(getGoodsUpTime());
		return;
	}
	assert(suspendEnd > suspendBegin);
	if (getGoodsUpTime() >= suspendBegin && getGoodsUpTime() < suspendEnd)
	{
		setWaitingExpire(suspendEnd);
		return;
	}
	setWaitingExpire(getGoodsUpTime());
}

void AuctionObject::initBidExpire()
{
	assert(getGoodsUpTime());
	const auto& gData = info::CSVAucitonParameterMgr::get().getGlobal();
	auto suspendBegin = getSuspendTimeBegin();
	if (!suspendBegin)
	{
		setBidExpire(getGoodsUpTime() + gData.kGoodsUpLasting);
		return;
	}
	auto suspendEnd = getSuspendTimeEnd();
	if (!suspendEnd)
	{
		assert(false);
		setBidExpire(getGoodsUpTime() + gData.kGoodsUpLasting);
		return;
	}
	if (suspendEnd < suspendBegin)
	{
		assert(false);
		setBidExpire(getGoodsUpTime() + gData.kGoodsUpLasting);
		return;
	}
	setBidExpire(getGoodsUpTime() + gData.kGoodsUpLasting + suspendEnd - suspendBegin);
}

bool AuctionObject::isWaiting() const
{
	return GetCurrTime() < getWaitingExpire();
}

bool AuctionObject::isBade() const
{
	return _leadingBid;
}

bool AuctionObject::isBidding() const
{
	return !_biddingIndex.empty();
}

bool AuctionObject::isAvailiable() const
{
	auto suspendBegin = getSuspendTimeBegin();
	if (!suspendBegin) return true;
	auto suspendEnd = getSuspendTimeEnd();
	if (!suspendEnd)
	{
		assert(false);
		return true;
	}
	assert(suspendEnd > suspendBegin);
	auto curTime = clockerMgr->getLocalSec();
	return curTime < suspendBegin || curTime >= suspendEnd;
}

bool AuctionObject::isOutOfDate() const
{
	return getBidExpire() && GetCurrTime() >= getBidExpire();
}

uint32 AuctionObject::getNextPrice() const
{
	if (!getPrice())
	{
		return getReservePrice();
	}
	return cAuction::getNextPrice(getPrice());
}

uint32 AuctionObject::getSuspendTimeBegin() const
{
}

uint32 AuctionObject::getSuspendTimeEnd() const
{
}

uint32 AuctionObject::getPopularity() const
{
}

uint32 AuctionObject::getPriceRank() const
{
	return cAuction::getPriceRank(getPriceShown());
}

uint32 AuctionObject::getPriceShown() const
{
	if (!getPrice())
	{
		return getNextPrice();
	}
	return getPrice();
}

uint32 AuctionObject::getStatisticFeature() const
{
	if (!objectPtr())
	{
		return 0;
	}
	return cAuction::getStatisticFeature(*objectPtr());
}

uint32 AuctionObject::allocBidId()
{
	if (_bidId == std::numeric_limits<uint32>::max())
	{
		return 0;
	}
	return ++_bidId;
}

void AuctionObject::__clearRelations()
{
	client::SecondAuction_Ntf_BidUpdated ntf;
	auto& bidProto = *ntf.mutable_bid();
	bidProto.set_objectid(getObjectId());
	auto data = ntf.SerializeAsString();
	for (auto& pair : _bids)
	{
		auto* bid = pair.second;
		if (auto* pUser = bid->userPtr())
		{
			pUser->sendCmdToMe(client::enClientFirst_Auction, client::enSecondAuciton_Ntf_BidUpdated, ntf);
			pUser->removeBid(*bid);
			pUser->saveDelay(record::enAU_Bid);
		}
		SAFE_DELETE(bid);
	}
	_bids.clear();

	{
		client::SecondAuction_Ntf_RmvSubscription ntf;
		ntf.set_objectid(getObjectId());
		auto data = ntf.SerializeAsString();
		for (auto& pair : _subscriptions)
		{
			auto* subscription = pair.second;
			if (auto* pUser = subscription->userPtr())
			{
				pUser->sendCmdToMe(client::enClientFrist_Auction, client::enSecondAuction_Ntf_RmvSubscription, ntf);
				pUser->removeSubscription(*subscription);
				pUser->saveDelay(reconrd::enAU_Subscripiton);
			}
			SAFE_DELETE(subscription);
		}
		_subscriptions.clear();
	}
}