#pragma once
#include "sdk.h"

namespace packet
{
	class Grid_Rect;
}

enum
{
	GridType_Tel = 0,				//传送点
	GridType_Max = 8,
};

//地图一个小范围
struct map_area_t
{
	zPos oLeftdown;
	zPos oRightup;
	bool ForeachGrid(std::function<bool(uint32 x, uint32 y)> func) const;
	bool ForeachGrid(std::function<bool(uint32 x, uint32 y, uint32 dx, uint32 dy)> func) const;
};

//固定npc内存数据(通过笔刷转换成内存数据)
struct map_fnpc_t
{
	uint32 uiTid = 0;
	map_area_t* pArea = nullptr;
	uint32 uiCircledir = 0;
	zPos oPos;
};

//传送点
struct map_tel_t : public map_area_t
{
	uint32 uiTargetMapTid = 0;
	zPos oTargetPos;
};

//动态怪物
struct map_mnpc_t
{
	uint32 uiTid = 0;
	map_area_t* pArea = nullptr;
	std::map<uint32, std::vector<zPos>> mapPos;
};

struct map_grid_t
{
	uint32 uiGridId = 0;
	map_tel_t* pTel = nullptr;
};

struct monster_group_info_t;
class CSceneMap : public zMap
{
public:
	using FNpcMapT = std::unordered_map<uint32, const map_fnpc_t*>;
	using TelMapI = std::unordered_map<uint32, const map_tel_t*>;
	using MNpcVecT = std::vector<const map_mnpc_t*>;
	using MNpcGroupMapT = std::unordered_map<uint32, MNpcVecT>;
public:
	CSceneMap(uint32 uiMapId);
	~CSceneMap();
	void Final();
	bool Init();
public:
	static constexpr uint32 kFullScreenIndex = 1;
public:
	bool IsFullScreen() const;
	zPosI Pos2zPosI(const zPos& oPos) const;
private:
	bool __InitMapInfo();
	bool __InitNpc();											//初始化固定npc
	void __FinalNpc();
	bool __InitTel();											//初始化传送点
	void __FinalTel();
	bool __InitMNpc();
	void __FinalMNpc();
private:
	map_grid_t* __CreateMapGrid(const uint32 x, const uint32 y);
	map_grid_t* __GetMapGrid(const uint32 uiGridId);
	BYTE& __GridInfo(const uint32 uiGridId);
	void __SetGridTel(const uint32 x, const uint32 y);
	bool __CheckPosValid(const packet::Grid_Rect& rect);						//检测范围坐标是否合法
	bool __FillMNpcGroupPos(map_mnpc_t& mnpc, const monster_group_info_t& config);	//从笔刷中读取怪物数据
private:
	FNpcMapT m_mapFNpc;
	TelMapI m_mapTels;
	MNpcGroupMapT m_mapStaticMNpcGroups;
	MNpcGroupMapT m_mapDynamicMNpcGroups;
	std::vector<uint8> m_vecInfos;
	std::unordered_map<uint32, map_grid_t*> m_mapGrids;
};