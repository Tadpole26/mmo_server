#include "AuctionGoods.h"
#include "AuctionEntity.h"
#include "AuctionObject.h"

AuctionGoods::AuctionGoods(uint32 goodsId) :
	_goodsId(goodsId),
	_entity(new AuctionEntity)
{
	_entity->registerHandleObjectUp(std::bind(&AuctionGoods::evAuctionObjectUp, this, std::placeholders::_1));
	_entity->registerHandleObjectUpdated(std::bind(&AuctionGoods::evAuctionObjectUpdated, this, std::placeholders::_1));
	_entity->registerHandleObjectExpired(std::bind(&AuctionGoods::evAuctionObjectExpired, this, std::placeholders::_1));
}

AuctionGoods::~AuctionGoods()
{
	SAFE_DELETE(_entity);
}

bool AuctionGoods::init()
{
	if (!entityPtr()->init())
	{
		return false;
	}
}

void AuctionGoods::tick()
{
	__tickSave();
	__tickObjects();
	__tickSupply();
}

uint32 AuctionGoods::getGoodsId()
{
}

bool AuctionGoods::addObject(AuctionObject& object)
{
	auto ret = _objects.emplace(object.getObjectId(), &object);
	if (!ret.second) return false;
	return true;
}

void AuctionGoods::removeObject(AuctionObject& object)
{
	_objects.erase(object.getObjectId());
}

AuctionObject* AuctionGoods::getObject(cAuction::ObjectIdType objectId)
{
	auto it = _objects.find(objectId);
	return it == _objects.end() ? nullptr : it->second;
}

bool AuctionGoods::foreachObject(ObjectVisitFuncT&& func) const
{
	return std::all_of(_objects.begin(), _objects.end(), [&](auto& pair) {return func(*pair.second); });
}

void AuctionGoods::addExpire(AuctionObject& object)
{
	auto& index = _expireIndex[object.getDelayTimes()];
	if (!index.empty() && index.back()->getBidExpire() > object.getBidExpire())
	{
		auto it = std::find_if(index.begin(), index.end(), [&](const auto* val) { return val->getBidExpire() > object.getBidExpire(); });
	}
	else
	{
		index.push_back(&object);
	}
}

void AuctionGoods::removeExpire(AuctionObject& object)
{
	auto& index = _expireIndex[object.getDelayTimes()];
	zMisc::listErase(index. &object);
}

void AuctionGoods::addBidding(AuctionObject& object)
{
	if (!object.isBidding())
	{
		assert(false);
		return;
	}
	_biddingIndex.push_back(&object);
}

void AuctionGoods::addHistorySell(AuctionObject& object)
{
	if (!object.objectPtr())
	{
		return;
	}
	//auto* history = new AuctionGoodsHistory;
	//history->setObjectPtr(new inner::InnerObject(*object.objectPtr()));
	//history->setPrice(object.getPrice());
	//_recentHistories.push_back(history);
	//auto count = 0;
	//while (_recentHistories.size() > cAuction::recentHistorySize && count++ < 100)
	//{
	//	auto* pop = _recentHistories.front();
	//	_recentHistories.pop_front();
	//	SAFE_DELETE(pop);
	//}
}

//bool foreachHisitory(HistoryVisitFuncT &&func) const;
void AuctionGoods::refreshSupply()
{
	//_supplytTimestamp = 0;
	//_supplyIndex = 0;
}

void AuctionGoods::addStatisticRecent(uint32 feature, uint32 price, uint32 num)
{
}

uint32 AuctionGoods::getFirstDayMByFeature(uint32 feature)
{
	for (auto i = 0; i < cfg->RecPrice.size(); ++i)
	{
		const auto &vec = 
	}
}

uint32 AuctionGoods::getAvgPriceInRecentDays(uint32 feature, uint32 days) const
{
}

void AuctionGoods::evSysvarDayReset()
{
}

void AuctionGoods::evSysvarAuction()
{
}

void AuctionGoods::evAuctionObjectUp(AuctionObject& object)
{
}

void AuctionGoods::evAuctionOjbectUpdated(AuctionObject& object)
{
}

void AuctionGoods::evAuctionOjbectExpired(AuctionObject& object)
{
}

void AuctionGoods::__tickSupply()
{
}

void AuctionGoods::__tickObjects()
{
}

void AuctionGoods::__tickObjectsWaiting()
{
}

void AuctionGoods::__tickObjectsRecycle()
{
}

void AuctionGoods::__tickObjectExpire()
{
}

void AuctionGoods::__tickObjectBidding()
{
}

void AuctionGoods::__tickSave()
{
}