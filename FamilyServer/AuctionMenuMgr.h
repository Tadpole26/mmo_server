#pragma once

#include "sdk.h"

class AuctionMenu;
class AuctionMenuMgr
{
public:
	using MenuVisitFuncT = std::function<bool(AuctionMenu&)>;
public:
	AuctionMenuMgr();
	~AuctionMenuMgr();
public:
	bool init();
	void final();
public:
	bool addMenu(AuctionMenu &menu);
	void removeMenu(AuctionMenu &menu);
	AuctionMenu* getMenu(uint32 menuId);
	bool foreachMenu(MenuVisitFuncT &&func);
private:
	AuctionMenu* __createMenu(uint32 menuId);
private:
	std::unordered_map<uint32, AuctionMenu*> _menus;
};

#define gAuctionMenuMgr Singleton<AuctionMenuMgr>::getInstance()
