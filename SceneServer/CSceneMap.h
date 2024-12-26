#pragma once
#include "sdk.h"

namespace packet
{
	class Grid_Rect;
}

struct map_grid_t
{
	uint32 uiGridId = 0;
};

class CSceneMap : public zMap
{
public:
	CSceneMap(uint32 uiMapId);
	~CSceneMap();
	void Final();
	bool Init();
private:
	bool __initMapInfo();
	bool __initNpc();											//初始化固定npc
	bool __finalNpc();
	bool __initTel();											//初始化传送点
	void __finalTel();
private:
	bool __checkPosValid(const packet::Grid_Rect& rect);
private:
	std::vector<uint8> m_vecInfos;
	std::unordered_map<uint32, map_grid_t*> m_mapGrids;
};