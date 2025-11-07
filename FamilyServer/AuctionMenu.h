#pragma once
#include "cAuction.h"
#include "sdk.h"

class AuctionFilterMenu;
class AuctionFilterSearch;
class AuctionGoods;
class AuctionObject;
class AuctionUser;
class AuctionMenu
{
public:
	AuctionMenu(uint32 menuId) {}
	virtual ~AuctionMenu();
public:
	using GoodsVisitFuncT = std::function<bool(AuctionGoods&)>;
	using FilterIdVisitFuncT = std::function<void(cAuction::FilterIdType)>;
	using FilterMenuVisitFuncT = std::function<void(const AuctionFilterMenu&)>;
	using FilterSearchVisitFuncT = std::function<void(const AuctionFilterSearch&)>;
public:
	virtual bool visitFilterMenu(AuctionUser &user, cAuction::FilterIdType filterId, uint32 orderType, FilterMenuVisitFuncT&& func) const;//玩家打开某一菜单
	virtual bool visitFilterSearch(cAuction::FilterIdType filterId, uint32 orderType, FilterSearchVisitFuncT&& func) const;//玩家搜索菜单
public:
	uint32 getMenuId() const { return _menuId; }
public:
	bool initFilters();
public:
	bool addGoods(AuctionGoods &goods);
	bool removeGoods(AuctionGoods &goods);
	AuctionGoods* getGoods(uint32 goodsId);
	bool foreachGoods(GoodsVisitFuncT &&func) const;
public:
	void addObject(AuctionObject &object);
	void removeObject(AuctionObject &object);
public:
	void foreachFilterId(AuctionObject &object, FilterIdVisitFuncT &&func);
public:
	struct FilterUnion
	{
		std::array<AuctionFilterMenu*, cAuction::OrderType_Size> menu;
		std::array<AuctionFilterSearch*, cAuction::OrderType_Size> search;
	};
private:
	uint32 _menuId = 0;	//对应配置菜单id
	std::unordered_map<uint32, AuctionGoods*> _goods;
private:
	std::unordered_map<cAuction::FilterIdType, FilterUnion> _filters;			//key:union(career,pos,level,quality) value:price_skiplist,time_skiplist
};