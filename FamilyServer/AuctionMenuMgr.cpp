#include "AuctionMenuMgr.h"
#include "AuctionMenu.h"

#include "AuctionConfig.h"

AuctionMenuMgr::AuctionMenuMgr()
{
}

AuctionMenuMgr::~AuctionMenuMgr()
{
}

bool AuctionMenuMgr::init()
{
	gAuctionCfg->foreachMenu([&](const config::auction_menu_t& menu) ->bool
		{
			auto* pMenu = __createMenu(menu.tid);
			if (!pMenu)
			{
				return false;
			}
			if (!addMenu(*pMenu))
			{
				SAFE_DELETE(pMenu);
				return false;
			}
			if (!pMenu->initFilters())
			{
				return false;
			}
			return true;
		});
	Log_Info("AuctionMenuMgr::init.success.%u", _menus.size());
}

void AuctionMenuMgr::final()
{
	for (auto& pair : _menus)
	{
		SAFE_DELETE(pair.second);
	}
	_menus.clear();
}

bool AuctionMenuMgr::addMenu(AuctionMenu& menu)
{
	auto ret = _menus.emplace(menu.getMenuId(), &menu);
	if (!ret.second)
	{
		return false;
	}
	return true;
}

void AuctionMenuMgr::removeMenu(AuctionMenu& menu)
{
	_menus.erase(menu.getMenuId());
}

AuctionMenu* AuctionMenuMgr::getMenu(uint32 menuId)
{
	auto it = _menus.find(menuId);
	return it == _menus.end() ? nullptr : it->second;
}

bool AuctionMenuMgr::foreachMenu(MenuVisitFuncT&& func)
{
	return std::all_of(_menus.begin(), _menus.end(), [&](auto& pair) {return func(*pair.second); });
}

AuctionMenu* AuctionMenuMgr::__createMenu(uint32 menuId)
{
	AuctionMenu* pMenu = nullptr;
	pMenu = new AuctionMenu(menuId);
	return pMenu;
}
