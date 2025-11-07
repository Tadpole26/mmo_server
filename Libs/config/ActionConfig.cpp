#include "AuctionConfig.h"

namespace config
{
	bool AuctionConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("auction_menu", std::bind(&AuctionConfig::__addAuctionMenu, this));
		InitSheet("auction_filter", std::bind(&AuctionConfig::__addAuctionFilter, this));

		return LoadCfgFromStr(strContent);

	}

	const auction_menu_t* AuctionConfig::getMenuConfig(uint32 menuId)
	{
		auto it = _auctionMenu.find(menuId);
		if (it == _auctionMenu.end()) return nullptr;
		return it->second;
	}

	const auction_filter_t* AuctionConfig::getFilterConfig(uint32 filterId)
	{
		auto it = _auctionFilter.find(filterId);
		if (it == _auctionFilter.end()) return nullptr;
		return it->second;
	}

	bool AuctionConfig::foreachMenu(std::function<bool(const auction_menu_t& menu)> func)
	{
		for (auto& pair : _auctionMenu)
		{
			auto* pMenuCfg = pair.second;
			if (!func(*pMenuCfg)) return false;
		}
		return true;
	}

	bool AuctionConfig::__addAuctionMenu()
	{
		auto* pMenuCfg = new auction_menu_t;
		assert(pMenuCfg != nullptr);
		pMenuCfg->tid = GetDataUInt("id");
		GetDataVec("show_option", pMenuCfg->showOption);
		return _auctionMenu.emplace(pMenuCfg->tid, pMenuCfg).second;
	}

	bool AuctionConfig::__addAuctionFilter()
	{
		auto* pFilterCfg = new auction_filter_t;
		assert(pFilterCfg != nullptr);
		pFilterCfg->tid = GetDataUInt("id");
		GetDataVec("option_content", pFilterCfg->optionContent);
		return _auctionFilter.emplace(pFilterCfg->tid, pFilterCfg).second;
	}

}
