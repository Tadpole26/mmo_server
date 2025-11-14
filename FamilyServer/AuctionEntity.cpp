#include "AuctionEntity.h"
#include "AuctionMenu.h"
#include "AuctionGoods.h"
#include "AuctionGoodsMgr.h"
#include "AuctionObject.h"
#include "AuctionObjectMgr.h"
#include "AucitonBid.h"

#include "AuctionConfig.h"

AuctionEntity::AuctionEntity()
{
}

AuctionEntity::~AuctionEntity()
{
}

bool AuctionEntity::init()
{
	_waitingIndex.sort([](auto* lval, auto* rval)
		{
			if (lval->getBidExpire() != rval->getBidExpire())
			{
				return lval->getBidExpire() < rval->getBidExpire();
			}
			return lval->getObjectId() < rval->getObjectId();
		});
	return true;
}

void AuctionEntity::registerHandleObjectUp(ObjectUpHandle&& handle)
{
	_handleObjectUp = handle;
}

void AuctionEntity::registerHandleObjectBid(ObjectBidHandle&& handle)
{
	_handleObjectBid = handle;
}

void AuctionEntity::registerHandleObjectUpdated(ObjectUpdatedHandle&& handle)
{
	_handleObjectUpdated = handle;
}

void AuctionEntity::registerHandleObjectExpired(ObjectExpiredHandle&& handle)
{
	_handleObjectExpired = handle;
}

bool AuctionEntity::addObject(AuctionObject& object)
{
	auto ret = _objects.emplace(object.getObjectId(), &object);
	if (!ret.second) return false;
	return true;
}

void AuctionEntity::removeObject(AuctionObject& object)
{
	_objects.erase(object.getObjectId());
}

AuctionObject* AuctionEntity::getObject(cAuction::ObjectIdType objectId)
{
	auto it = _objects.find(objectId);
	return it == _objects.end() ? nullptr : it->second;
}

bool AuctionEntity::foreachObject(ObjectVisitFuncT&& func) const
{
	return std::all_of(_objects.begin(), _objects.end(), [&](auto& pair) { return func(*pair.second); });
}

void AuctionEntity::addWaiting(AuctionObject& object)
{
	_waitingIndex.push_back(&object);
}

bool AuctionEntity::foreachWaiting(ObjectVisitFuncT&& func)
{
	return std::all_of(_waitingIndex.begin(), _waitingIndex.end(), [&](auto* object) { return func(*object); });
}

void AuctionEntity::addRecycle(AuctionObject& object)
{
	_recycleIndex.push_back(&object);
}

void AuctionEntity::removeRecycle(AuctionObject& object)
{
	zMisc::listErase(_recycleIndex, &object);
}

bool AuctionEntity::foreachRecycle(ObjectVisitFuncT&& func) const
{
	return std::all_of(_recycleIndex.begin(), _recycleIndex.end(), [&](auto* object) {return func(*object); });
}

AuctionObject* AuctionEntity::goodsUp(uint32 goodsId, uint32 price, const inner::InnerObject& objData, uint32 upTime)
{
	auto* goods = gAuctionGoodsMgr->getGoods(goodsId);
	if (!goods)
	{
		return nullptr;
	}
	auto* pGoodsCfg = gAuctionCfg->getGoodsConfig(goodsId);
	if (!pGoodsCfg)
	{
		return nullptr;
	}
	if (price < pGoodsCfg->lowPrice)
	{
		return nullptr;
	}
	auto* object = gAuctionGoodsMgr->createObject();
	if (!object)
	{
		return nullptr;
	}
	if (!addObject(*object))
	{
		return nullptr;
	}
	object->setEntityPtr(this);			//上架所属
	object->setGoodsId(goodsId);		//属于商品id
	object->setObjectPtr(new inner::InnerObject(objData));//实际道具属性
	object->setReservePrice(price);//上架价格
	object->setPrice(0);//当前实际价格
	object->setGoodsUpTime(upTime);//上架时间
	object->initWaitingExpire();
	object->initBidExpire();
	addWaiting(*object);
	return object;
}

void AuctionEntity::objectUp(AuctionObject& object)
{
	auto* goods = gAuctionGoodsMgr->getGoods(object.getGoodsId());
	if (!goods)
	{
		return;
	}
	if (!goods->addObject(object))
	{
		return;
	}
	object.setGoodPtr(goods);
	zMisc::listErase(_waitingIndex, &object);
	goods->addExpire(object);
	if (auto* menu = goods->menuPtr())
	{
		menu->addObject(object);
	}

	object.foreachBid([&](auto& bid) 
		{
			if (!bid.isBidding()) return false;
			object.addBidding(bid);
			Log_Debug("objectUp.setBidding");
			return true;
		});
	object.init();
	if (object.isBidding())
	{
		goods->addBidding(object);
	}
	if (_handleObjectUp)
	{
		_handleObjectUp(object);
	}
}

void AuctionEntity::objectBid(AuctionObject& object, uint32 num)
{
	if (_handleObjectBid)
	{
		_handleObjectBid(object, num);
	}
}

void AuctionEntity::objectUpdated(AuctionObject& object)
{
	if (_handleObjectUpdated)
	{
		_handleObjectUpdated(object);
	}
}

void AuctionEntity::objectExpired(AuctionObject& object)
{
	if (_handleObjectExpired)
	{
		_handleObjectExpired(object);
	}
}

std::pair<bool, inner::InnerObject> AuctionEntity::goodsDown(cAuction::ObjectIdType objectId)
{
	std::pair<bool, inner::InnerObject> ret{ false, {}};
	auto* object = getObject(objectId);
	if (!object)
	{
		return ret;
	}
	if (object->isBade())
	{
		return ret;
	}
	object->evAuctionObjectDown();
	auto* goods = object->goodsPtr();
	if (goods)
	{
		if (auto* menu = goods->menuPtr())
		{
			menu->removeObject(*object);
		}
		if (!object->isOutOfDate())
		{
			goods->removeExpire(*object);
		}
		goods->removeObject(*object);
		object->setGoodsPtr(nullptr);
	}
	ret.first = true;
	if (object->objectPtr())
	{
		ret.second = std::move(*object->objectPtr());
	}
	if (object->isWaiting())
	{
		zMisc::listErase(_waitingIndex, object);
	}
	removeObject(*object);
	addRecycle(*object);
	return ret;
}