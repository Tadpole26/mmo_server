#pragma once
#include "sdk.h"
#include "cAuction.h"

class AuctionEntity;
class AuctionMenu;
class AuctionObject;

class AuctionGoods
{
public:
	using ObjectVisitFuncT = std::function<bool(AuctionObject&)>;
	//using HistoryVisitFuncT = std::function<bool(const AuctionGoodsHistory&)>
public:
	struct StatisticRecent
	{
		uint32 recentBeginTime = 0;
		uint64 dailySaleVolume = 0;
		uint32 dailySaleNum = 0;
		std::vector<uint32> avgPriceList;
	};
public:
	AuctionGoods(uint32 goodsId);
	~AuctionGoods();
public:
	//void serialize(record::AuctionSystemBase &out) const;
	//void serialize(record::AuctionSystemObjectList &out) const;
	//void serialize(record::AuctionSystemStaticstic &out) const;
	//void unserialize(const record::AuctionSystemBase &in);
	//void unserialize(const record::AuctionSystemObjectList &in);
	//void unserialize(const record::AuctionSystemStatistic& in);
public:
	bool init();
	void tick();
public:
	uint32 getGoodsId();
public:
	bool addObject(AuctionObject &object);
	void removeObject(AuctionObject &object);
	AuctionObject* getObject(cAuction::ObjectIdType objectId);
	bool foreachObject(ObjectVisitFuncT &&func) const;
	void addExpire(AuctionObject &object);
	void removeExpire(AuctionObject &object);
	void addBidding(AuctionObject &object);
public:
	void addHistorySell(AuctionObject &object);
	//bool foreachHisitory(HistoryVisitFuncT &&func) const;
	void refreshSupply();
public:
	void addStatisticRecent(uint32 feature, uint32 price, uint32 num);
	uint32 getFirstDayMByFeature(uint32 feature);
	uint32 getAvgPriceInRecentDays(uint32 feature, uint32 days) const;
public:
	void evSysvarDayReset();
	void evSysvarAuction();
public:
	void evAuctionObjectUp(AuctionObject &object);
	void evAuctionObjectUpdated(AuctionObject &object);
	void evAuctionObjectExpired(AuctionObject &object);
private:
	void __tickSupply();
	void __tickObjects();
	void __tickObjectsWaiting();
	void __tickObjectsRecycle();
	void __tickObjectExpire();
	void __tickObjectBidding();
	void __tickSave();
public:
	const AuctionEntity* entityPtr() const { return _entity; }
	AuctionEntity* entityPtr() { return _entity; }
	void setEntityPtr(AuctionEntity* entity) { _entity = entity; }
	const AuctionMenu* menuPtr() const { return _menu; }
	AuctionMenu* menuPtr() { return _menu; }
	void setMenuPtr(AuctionMenu* menu) { _menu = menu; }
private:
	uint32 _goodsId = 0;
	AuctionEntity* _entity = nullptr;
	AuctionMenu* _menu = nullptr;
	uint32 _tickGroup = 0;
	std::unordered_map<cAuction::ObjectIdType, AuctionObject*> _objects;
	std::map<uint32, std::list<AuctionObject*>> _expireIndex;
	std::map<uint32, std::unordered_map<cAuction::ObjectIdType, AuctionObject*>> _priceIndex;
	std::list<AuctionObject*> _biddingIndex;
	
};
