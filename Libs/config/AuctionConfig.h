#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct auction_menu_t
	{
		uint32 tid = 0;
		std::vector<uint32> showOption;
	};

	struct auction_filter_t
	{
		uint32 tid = 0;			
		std::vector<uint32> optionContent;
	};

	struct acution_goods_t
	{
		uint32 tid = 0;
		uint32 itemId = 0;
		uint32 lowPrice = 0;//起拍价最低
		uint32 highPrice = 0;//起拍价最高
	};

	class AuctionConfig : public cLoadBase
	{
	public:
		enum
		{
			FilterType_None = 0,
			FilterType_Career = 1,		//职业
			FilterType_EquipPos = 2,	//装备部位
			FilterType_Level = 3,		//等级
			FilterType_Quality = 4,		//品质
			FilterType_CardPos = 5,		//卡片部位
			FilterType_EquipProp = 6,	//搜索-装备属性(id读属性表)
			FilterType_CardProp = 7,	//搜索-卡片属性(id读属性表)
		};
	public:
		bool LoadConfig(const std::string& strContent) override;
		
		const auction_menu_t* getMenuConfig(uint32 menuId);
		const auction_filter_t* getFilterConfig(uint32 filterId);
		bool foreachMenu(std::function<bool (const auction_menu_t& menu)> func);
		const acution_goods_t* getGoodsConfig(uint32 goodsId);
	private:
		bool __addAuctionMenu();
		bool __addAuctionFilter();
	private:
		std::unordered_map<uint32_t, const auction_menu_t*> _auctionMenu;
		std::unordered_map<uint32_t, const auction_filter_t*> _auctionFilter;
		std::unordered_map<uint32_t, const acution_goods_t*> _auctonGoods;
	};
}

#define gAuctionCfg Singleton<config::AuctionConfig>::getInstance()
