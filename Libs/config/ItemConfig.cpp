#include "ItemConfig.h"

namespace config
{
	bool ItemConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("item_info", std::bind(&ItemConfig::AddItemInfo, this));

		return LoadCfgFromStr(strContent);
	}

	bool ItemConfig::LoadConfigByFile(const std::string& fileName)
	{
		InitSheet("item_info", std::bind(&ItemConfig::AddItemInfo, this));

		return LoadCfgFromXml(fileName.c_str());
	}

	bool ItemConfig::AddItemInfo()
	{
		auto* pItemCfg = new item_info_t;
		assert(pItemCfg != nullptr);
		pItemCfg->tid = GetDataUInt("id");
		pItemCfg->type = GetDataUInt("type");							
		_allItemCfg[pItemCfg->tid] = pItemCfg;
		return true;
	}

	const item_info_t* ItemConfig::getItemConfig(const uint32 tid) const
	{
		auto iter = _allItemCfg.find(tid);
		if (iter == _allItemCfg.end())
			return nullptr;
		return iter->second;
	}
}
