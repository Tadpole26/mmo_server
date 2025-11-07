#include "AuctionMenu.h"
#include "AuctionFilterMenu.h"
#include "AuctionFilterSearch.h"

#include "AuctionConfig.h"
#include "ItemConfig.h"

AuctionMenu::AuctionMenu(uint32 menuId) : _menuId(menuId)
{
}

AuctionMenu::~AuctionMenu()
{
	for (auto& pair : _filters)
	{
		auto& filters = pair.second;
		for (auto i = 0; i < filters.menu.size(); ++i)
		{
			SAFE_DELETE(filters.menu[i]);
		}
		for (auto i = 0; i < filters.search.size(); ++i)
		{
			SAFE_DELETE(filters.search[i]);
		}
	}
}

bool AuctionMenu::visitFilterMenu(AuctionUser& user, cAuction::FilterIdType filterId, uint32 orderType, FilterMenuVisitFuncT&& func) const
{
	if (!cAuction::isValidOrderType(orderType)) return false;
	auto it = _filters.find(filterId);
	if (it == _filters.end()) return false;
	const auto& filters = it->second;
	auto* filter = filters.menu[orderType - 1];
	if (!filter) return false;
	func(*filter);
	return true;
}

bool AuctionMenu::visitFilterSearch(cAuction::FilterIdType filterId, uint32 orderType, FilterSearchVisitFuncT&& func) const
{
	auto it = _filters.find(filterId);
	if (it == _filters.end()) return false;
	const auto& filters = it->second;
	auto* filter = filters.search[orderType - 1];
	if (!filter) return false;
	func(*filter);
	return true;
}

bool AuctionMenu::initFilters()
{
	std::function<bool(uint32)> func;
	uint32 career = 0;
	uint32 pos = 0;
	uint32 level = 0;
	uint32 quality = 0;
	func = [&](uint32 index)->bool
		{
			auto* pMenuCfg = gAuctionCfg->getMenuConfig(_menuId);
			assert(pMenuCfg != nullptr);
			if (index >= pMenuCfg->showOption.size())
			{
				auto hashId = cAuction::unionFilterId(career, pos, level, quality);
				auto it = _filters.find(hashId);
				if (it != _filters.end())
				{
					Log_Error("initFilters().!hashId,%u,%u,%u,%u", career, pos, level, quality);
					assert(false);
					return false;
				}
				auto& filters = _filters[hashId];
				for (auto i = cAuction::OrderType_None + 1; i < cAuction::OrderType_Max; ++i)
				{
					auto* filter = new AuctionFilterMenu(i);
					filters.menu[i - 1] = filter;
				}
				for (auto i = cAuction::OrderType_None + 1; i < cAuction::OrderType_Max; ++i)
				{
					auto* filter = new AuctionFilterSearch(i);
					filters.search[i - 1] = filter;
				}
				return true;
			}
			auto* pFilterCfg = gAuctionCfg->getFilterConfig(pMenuCfg->showOption[index]);
			if (!pFilterCfg)
			{
				Log_Error("initFilters().!cfgFilter,%u", pMenuCfg->showOption[index]);
				return false;
			}
			uint32* val = nullptr;
			switch (pMenuCfg->showOption[index])
			{
			case config::AuctionConfig::FilterType_Career: val = &career;	break;
			case config::AuctionConfig::FilterType_Level: val = &level;	break;
			case config::AuctionConfig::FilterType_Quality: val = &quality;	break;
			case config::AuctionConfig::FilterType_EquipPos: 
			case config::AuctionConfig::FilterType_CardPos: 
				val = &pos;
				break;
			}
			for (auto i = 0; i < pFilterCfg->optionContent.size(); ++i)
			{
				if (val)
				{
					*val = pFilterCfg->optionContent[i];
				}
				if (!func(index + 1)) return false;
			}
			return true;
		};
	if (!func(0))
	{
		Log_Error("initFilters.fail");
		return false;
	}
	Log_Info("initFilters.success,%u",_filters.size());
	return true;
}

bool AuctionMenu::addGoods(AuctionGoods& goods)
{
	auto ret = _goods.emplace(goods.getGoodsId(), &goods);
	if (!ret.second)
	{
		return false;
	}
	return true;
}

bool AuctionMenu::removeGoods(AuctionGoods& goods)
{
	_goods.erase(goods.getGoodsId());
}

AuctionGoods* AuctionMenu::getGoods(uint32 goodsId)
{
	auto it = _goods.find(goodsId);
	return it == _goods.end() ? nullptr : it->second;
}

bool AuctionMenu::foreachGoods(GoodsVisitFuncT&& func) const
{
	return std::all_of(_goods.begin(), _goods.end(), [&](auto& pair) {return func(*pair.second);
 });
}

void AuctionMenu::addObject(AuctionObject& object)
{
	foreachFilterId(object, [&](auto filterHashId) 
		{
			auto it = _filters.find(filterHashId);
			if (it == _filters.end()) return;
			auto& filters = it->second;
			for (auto i = 0; i < filters.menu.size(); ++i)
			{
				auto* filter = filters.menu[i];
				filter->addObject(object);
			}
			for (auto i = 0; i < filters.search.size(); ++i)
			{
				auto* filter = filters.search[i];
				filter->addObject(object);
			}
		});
}

void AuctionMenu::removeObject(AuctionObject& object)
{
	foreachFilterId(object, [&](auto filterHashId)
		{
			auto it = _filters.find(filterHashId);
			if (it == _filters.end()) return;
			auto& filters = it->second;
			for (auto i = 0; i < filters.menu.size(); ++i)
			{
				auto* filter = filters.menu[i];
				filter->removeObject(object);
			}
			for (auto i = 0; i < filters.search.size(); ++i)
			{
				auto* filter = filters.search[i];
				filter->removeObject(object);
			}
		});
}

void AuctionMenu::foreachFilterId(AuctionObject& object, FilterIdVisitFuncT&& func)
{
	auto* goods = object.goodsPtr();
	if (!goods)
	{
		assert(false);
		return;
	}
	auto foreachCareer = [&](std::function<void(uint32)>&& func)
		{
			if (goods->cfg->Career.empty())
			{
				auto * pFilterCfg = gAuctionCfg->getFilterConfig(config::AuctionConfig::FilterType_Career);
				if (!pFilterCfg) return;
				for (auto i = 0; i < pFilterCfg->optionContent.size(); ++i)
				{
					func(pFilterCfg->optionContent[i]);
				}
			}
			else
			{
				for (auto i = 0; i < goods->cfg->Career.size(); ++i)
				{
					func(goods->cfg->Career[i]);
				}
				func(0);
			}
		};
	auto foreachPos = [&](std::function<void(uint32)>&& func)
		{
			if (!object.objectPtr()) return;
			if (!object.objectPtr()->has_item())
			{
				func(0);
				return;
			}
			const auto& item = object.objectPtr()->item().item();
			if (item.has_equip())
			{
				auto* pEquipCfg = gItemCfg->getEquipCfg(item.base().tid());
				if (!pEquipCfg) return;
				func(pEquipCfg->weaponCategory);
			}
			else if (item.has_card())
			{
				const auto* pCardCfg = gItemCfg->getCardCfg(item.base().tid());
				if (!pCardCfg) return;
				func(pCardCfg->type);
			}
			func(0);
		};
	auto foreachLevel = [&](std::function<void(uint32)>&& func)
		{
			if (goods->cfg->level)
			{
				func(goods->cfg->Level);
				func(0);
			}
			else
			{
				auto* pFilterCfg = gAuctionCfg->getFilterConfig(config::AuctionConfig::FilterType_Career);
				if (!pFilterCfg) return;
				for (auto i = 0; i < pFilterCfg->optionContent.size(); ++i)
				{
					func(pFilterCfg->optionContent[i]);
				}
			}
		};
	auto foreachQuality = [&](std::function<void(uint32)>&& func)
		{
			if (!object.objectPtr()) return;
			if (!object.objectPtr()->has_item())
			{
				func(0);
				return;
			}
			const auto& item = object.objectPtr()->item().item();
			if (!item.has_equip())
			{
				func(0);
				return;
			}
			auto quality = cObject::getObjectQuality(*object.objectPtr());
			if (quality)
			{
				func(quality);
			}
			func(0);
		};
	foreachCareer([&](auto career)
		{
			foreachPos([&](auto pos)
				{
					foreachLevel([&](auto level) 
						{
							foreachQuality([&](auto quality) 
								{
									auto hashId = cAuction::unionFilterId(career, pos, level, quality);
									func(hashId);
								});
						});
				});
		});
}