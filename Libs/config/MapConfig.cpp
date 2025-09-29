#include "MapConfig.h"

namespace config
{
	bool MapConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("map_info", std::bind(&MapConfig::AddMapInfo, this));

		return LoadCfgFromStr(strContent);
	}

	bool MapConfig::LoadConfigByFile(const std::string& fileName)
	{
		InitSheet("map_info", std::bind(&MapConfig::AddMapInfo, this));

		return LoadCfgFromXml(fileName.c_str());
	}

	bool MapConfig::AddMapInfo()
	{
		auto* pMapCfg = new map_info_t;
		assert(pMapCfg != nullptr);
		pMapCfg->tid = GetDataUInt("id");
		pMapCfg->strMapName = GetDataChr("comment");							//地图名
		pMapCfg->uiMapLevel = GetDataUInt("level");							//地图要求等级
		pMapCfg->bStatic = GetDataBool("static");								//是否是静态地图
		pMapCfg->uiWidth = GetDataUInt("width");								//地图宽度X(8的倍数)
		pMapCfg->uiHeight = GetDataUInt("height");							//地图高度X(8的倍数)
		GetDataPos("born_point", pMapCfg->oBornPoint);							//出生坐标
		pMapCfg->uiBornDirect = GetDataUInt("direct");						//出生朝向
		GetDataPos("reborn_point", pMapCfg->oRebornPoint);
		pMapCfg->uiRebornDirect = GetDataUInt("reborn_direct");						//重生朝向
		pMapCfg->navmesh = GetDataUInt("height");							//寻路文件
		_allMapCfg[pMapCfg->tid] = pMapCfg;
		return true;
	}

	const map_info_t* MapConfig::getMapConfig(const uint32 tid) const
	{
		auto iter = _allMapCfg.find(tid);
		if (iter == _allMapCfg.end())
			return nullptr;
		return iter->second;
	}

	bool MapConfig::foreach(std::function<bool(map_info_t* pConfig)> func) const
	{
		for (auto& pair : _allMapCfg)
		{
			auto* pMapCfg = pair.second;
			if (!func(pMapCfg)) return false;
		}
		return true;
	}
}
