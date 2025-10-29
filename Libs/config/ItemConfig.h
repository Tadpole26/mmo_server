#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct item_info_t
	{
		uint32 tid = 0;
		uint32 type = 0;
	};

	class ItemConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;
		bool LoadConfigByFile(const std::string& fileName) override;

		const item_info_t* getItemConfig(const uint32 tid) const;
	private:
		bool AddItemInfo();

	private:
		std::map<uint32_t, item_info_t*> _allItemCfg;
	};
}
#define gItemCfg Singleton<config::ItemConfig>::getInstance()
