#pragma once

#include "cAuction.h"
#include "sdk.h"

namespace inner
{
	class InnerObject;
}

class AuctionObject;
class AuctionEntity
{
public:
	using ObjectVisitFuncT = std::function<bool(AuctionObject&)>;
	using ObjectUpHandle = std::function<void(AuctionObject&)>;
	using ObjectBidHandle = std::function<void(AuctionObject&, uint32)>;
	using ObjectUpdatedHandle = std::function<void(AuctionObject&)>;
	using ObjectExpiredHandle = std::function<void(AuctionObject&)>;
	using LogHeadHandle = std::function<std::string()>;
	using ZoneIdHandle = std::function<uint32()>;
public:
	AuctionEntity();
	~AuctionEntity();
public:
	bool init();
public:
	void registerHandleObjectUp(ObjectUpHandle &&handle);
	void registerHandleObjectBid(ObjectBidHandle &&handle);
	void registerHandleObjectUpdated(ObjectUpdatedHandle &&handle);
	void registerHandleObjectExpired(ObjectExpiredHandle &&handle);
public:
	bool addObject(AuctionObject &object);
	void removeObject(AuctionObject &object);
	AuctionObject* getObject(cAuction::ObjectIdType objectId);
	bool foreachObject(ObjectVisitFuncT&& func) const;
public:
	void addWaiting(AuctionObject &object);
	bool foreachWaiting(ObjectVisitFuncT&& func);
	void addRecycle(AuctionObject &object);
	void removeRecycle(AuctionObject &object);
	bool foreachRecycle(ObjectVisitFuncT &&func) const;
public:
	AuctionObject* goodsUp(uint32 goodsId, uint32 price, const inner::InnerObject &objData, uint32 upTime);
	void objectUp(AuctionObject &object);
	void objectBid(AuctionObject &object, uint32 num);
	void objectUpdated(AuctionObject &object);
	void objectExpired(AuctionObject &object);
	std::pair<bool, inner::InnerObject> goodsDown(cAuction::ObjectIdType objectId);
private:
	ObjectUpHandle _handleObjectUp;
	ObjectBidHandle _handleObjectBid;
	ObjectUpdatedHandle _handleObjectUpdated;
	ObjectExpiredHandle _handleObjectExpired;
	std::map<cAuction::ObjectIdType, AuctionObject*> _objects;		//挂在玩家身上为玩家上架,挂在goods身上为系统上架
	std::list<AuctionObject*> _waitingIndex;	//等待上架列表
	std::list<AuctionObject*> _recycleIndex;
};