#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct map_info_t
	{
		uint32 uiUid = 0;
		std::string strMapName;							//地图名
		uint32 uiMapLevel = 0;							//地图要求等级
		bool bStatic = true;							//是否是静态地图
		uint32 uiTiny = 0;								//是否是微型地图
		uint32 uiWidth = 0;								//地图宽度X(8的倍数)
		uint32 uiHeight = 0;							//地图高度X(8的倍数)
		zPos oBornPoint;								//出生坐标
		uint32 uiBornDirect = 0;						//出生朝向
		zPos oRebornPoint;								//重生坐标
		uint32 uiRebornDirect = 0;						//重生朝向
		uint32 uiNavmesh = 0;							//寻路文件
		uint32 maxLine = 64;							//该地图允许开最大线数量
	};


	class MapConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;

		map_info_t* GetMapConfig(const uint32 tid);
	private:
		bool AddMapInfo();

	private:
		std::map<uint32_t, map_info_t> _mapInfoCfg;
	};
}
#define gMapCfg Singleton<config::MapConfig>::getInstance()
