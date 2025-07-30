#pragma once
#include "sdk.h"

namespace packet
{
	class Grid_Rect;
}

namespace config
{
	class monster_group_info_t;
}

enum
{
	GridType_Tel = 0,				//���͵�
	GridType_Max = 8,
};

//��ͼһ��С��Χ
struct map_area_t
{
	zPos leftdown;
	zPos rightup;
	bool foreachGrid(std::function<bool(uint32 x, uint32 y)> func) const;
	bool foreachGrid(std::function<bool(uint32 x, uint32 y, uint32 dx, uint32 dy)> func) const;
};

//�̶�npc�ڴ�����(ͨ����ˢת�����ڴ�����)
struct map_fnpc_t
{
	uint32 tid = 0;
	map_area_t* area = nullptr;
	uint32 circledir = 0;
	zPos pos;
};

//���͵�
struct map_tel_t : public map_area_t
{
	uint32 targetMapTid = 0;
	zPos targetPos;
};

//��̬����
struct map_mnpc_t
{
	uint32 tid = 0;
	map_area_t* area = nullptr;
	std::map<uint32, std::vector<zPos>> posList;
};

struct map_grid_t
{
	uint32 gridId = 0;
	map_tel_t* tel = nullptr;
};

struct monster_group_info_t;
class SceneMap : public zMap
{
public:
	using MapInfoVecT = std::vector<BYTE>;
	using MapGridMapT = std::unordered_map<uint32, map_grid_t*>;
	using MapTelMapT = std::unordered_map<uint32, const map_tel_t*>;
	using MapFNpcMapT = std::unordered_map<uint32, const map_fnpc_t*>;
	using MapMNpcGroupInsVecT = std::vector<const map_mnpc_t*>;
	using MapMNpcGroupIdMapT = std::unordered_map<uint32, MapMNpcGroupInsVecT>;
public:
	static constexpr uint32 kFullScreenIndex = 1;
public:
	SceneMap(uint32 uiMapId);
	~SceneMap();
	void final();
	bool init();
public:
	bool isFullScreen() const;
	zPosI pos2zPosI(const zPos& oPos) const;
private:
	bool __initMapInfo();
	bool __initNpc();											//��ʼ���̶�npc
	void __finalNpc();
	bool __initTel();											//��ʼ�����͵�
	void __finalTel();
	bool __initMNpc();
	void __finalMNpc();
private:
	map_grid_t* __createMapGrid(const uint32 x, const uint32 y);
	map_grid_t* __getMapGrid(const uint32 uiGridId);
	BYTE& __gridInfo(const uint32 uiGridId);
	void __setGridTel(const uint32 x, const uint32 y);
	bool __checkPosValid(const packet::Grid_Rect& rect);									//��ⷶΧ�����Ƿ�Ϸ�
	bool __fillMNpcGroupPos(map_mnpc_t& mnpc, const config::monster_group_info_t& config);	//�ӱ�ˢ�ж�ȡ��������
private:
	MapInfoVecT _infos;
	MapGridMapT _grids;
	MapTelMapT _tels;
	MapFNpcMapT _fnpcs;
	MapMNpcGroupIdMapT _staticMNpcGroups;
	MapMNpcGroupIdMapT _dynamicMNpcGroups;
};