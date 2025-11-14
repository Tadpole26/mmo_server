#pragma once

#include "cAuction.h"

namespace inner
{
	class InnerObject;
}

namespace record
{
	class AuctionObject;
	class AuctionUserObject;
}

namespace common
{
	class AuctionUserGoods;
	class AuctionObject;
}

class AuctionUser;
class AuctionEntity;
class AuctionGoods;
class AuctionBid;
class AuctionSubscription;

class AuctionObject
{
public:
	using BidVisitFuncT = std::function<bool(AuctionBid&)>;
	using SubscriptionVisitFuncT = std::function<bool(AuctionSubscription&)>;
public:
	AuctionObject(cAuction::ObjectIdType id);
	~AuctionObject();
public:
	uint64 getObjectNum() const;
public:
	void fill(common::AuctionObject& out) const {}
	void fill(common::AuctionUserGoods& out) const {}
	void serialize(record::AuctionObject& out) const {}
	void serialize(record::AuctionUserObject& out) const {}
	void unserialize(const record::AuctionObject& in) {}
	void unserialize(const record::AuctionUserObject& in) {}
public:
	bool init();
	void tick();
public:
	void evAuctionObjectExpired();
	void evAuctionObjectDown();
public:
	bool addBid(AuctionBid& bid);
	bool removeBid(AuctionBid& bid);
	AuctionBid* getBid(zRoleIdType userId);
	bool foreachBid(BidVisitFuncT&& func) const;
public:
	void addBidding(AuctionBid& bid);
public:
	bool addSubscription(AuctionSubscription& subscription);
	void removeSubscription(AuctionSubscription& subscription);
	AuctionSubscription* getSubscription(zRoleIdType userId);
	bool foreachSubscription(SubscriptionVisitFuncT&& func) const;
public:
	void initWaitingExpire();
	void initBidExpire();
public:
	bool isWaiting() const;
	bool isBade() const;
	bool isBidding() const;
	bool isAvailiable() const;
	bool isOutOfDate() const;
public:
	uint32 getNextPrice() const;
	uint32 getSuspendTimeBegin() const;
	uint32 getSuspendTimeEnd() const;
	uint32 getSubscriptionCount() const { return _subscriptions.size(); }
	uint32 getPopularity() const;
	uint32 getPriceRank() const;
	uint32 getPriceShown() const;
	uint32 getStatisticFeature() const;
public:
	uint32 allocBidId();
private:
	void __clearRelations();
public:
	cAuction::ObjectIdType getObjectId() const { return _id; }
	uint32 getGoodsId() const { return _goodsId; }
	void setGoodsId(uint32 goodsId) { _goodsId = goodsId; }
	const AuctionEntity* entityPtr() const { return _entity; }
	AuctionEntity* entityPtr() { return _entity; }
	const AuctionGoods* goodsPtr() const { return _goods; }
	AuctionGoods* goodsPtr() { return _goods; }
	void setGoodsPtr(AuctionGoods* goods) { _goods = goods; }
	inner::InnerObject* objectPtr() const { return _object; }
	void setObjectPtr(inner::InnerObject* object) { _object = object; }
	uint32 getReservePrice() const { return _reservePrice; }
	void setReservePrice(uint32 val) { _reservePrice = val; }
	uint32 getPrice() const { return _price; }
	void setPrice(uint32 price) { _price = price; }
	uint32 getGoodsUpTime() const { return _goodsUpTime; }
	void setGoodsUpTime(uint32 val) { _goodsUpTime = val; }
	uint32 getWaitingExpire() const { return _waitingExpire; }
	void setWaitingExpire(uint32 val) { _waitingExpire = val; }
	uint32 getBidExpire() const { return _bidExpire; }
	void setBidExpire(uint32 val) { _bidExpire = val; }
	uint32 getDelayTimes() const { return _delayTimes; }
	void setDelayTimes(uint32 val) { _delayTimes = val; }
	uint32 getBidId() const { return _bidId; }
	void setBidId(uint32 bidId) { _bidId = bidId; }
	const AuctionBid* leadingBidPtr() const { return _leadingBid; }
	AuctionBid* leadingBidPtr() { return _leadingBid; }
	AuctionBid* setLeadingBidPtr(AuctionBid* leadingBid) { _leadingBid = leadingBid; }
private:
	cAuction::ObjectIdType _id = 0;
	uint32 _goodsId = 0;
	AuctionEntity* _entity = nullptr;		//所属对象
	AuctionGoods* _goods = nullptr;			//所属商品
	inner::InnerObject* _object = nullptr;	//实际道具对象
	uint32 _reservePrice = 0;				//起拍价格
	uint32 _price = 0;						//当前价格
	uint32 _goodsUpTime = 0;
	uint32 _waitingExpire = 0;
	uint32 _bidExpire = 0;
	uint32 _delayTimes = 0;
	uint32 _bidId = 0;						//自增竞拍id
	std::unordered_map<zRoleIdType, AuctionBid*> _bids;//所有竞价信息
	AuctionBid* _leadingBid = nullptr;
	std::list<AuctionBid*> _biddingIndex;
	std::unordered_map<zRoleIdType, AuctionSubscription*> _subscriptions;

};
