#include "MapConfig.h"

namespace config
{
	bool MapConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("map_info", std::bind(&MapConfig::AddMapInfo, this));

		return LoadCfgFromStr(strContent);

	}

	bool MapConfig::AddMapInfo()
	{
		uint32 uiUid = GetDataUInt("id");
		map_info_t& stElem = _MapInfoCfg[uiUid];
		stElem.uiUid = GetDataUInt("id");
		stElem.strMapName = GetDataChr("comment");							//地图名
		stElem.uiMapLevel = GetDataUInt("level");							//地图要求等级
		stElem.bStatic = GetDataBool("static");								//是否是静态地图
		stElem.uiWidth = GetDataUInt("width");								//地图宽度X(8的倍数)
		stElem.uiHeight = GetDataUInt("height");							//地图高度X(8的倍数)
		GetDataPos("born_point", stElem.oBornPoint);							//出生坐标
		stElem.uiBornDirect = GetDataUInt("direct");						//出生朝向
		GetDataPos("reborn_point", stElem.oRebornPoint);
		stElem.uiRebornDirect = GetDataUInt("reborn_direct");						//重生朝向
		stElem.uiNavmesh = GetDataUInt("height");							//寻路文件
		return true;
	}

	map_info_t* MapConfig::GetMapConfig(const uint32 tid)
	{
		auto iter = _MapInfoCfg.find(tid);
		if (iter == _MapInfoCfg.end())
			return nullptr;
		return &(iter->second);
	}
}
