#pragma once

#include "sdk.h"

enum eViewType
{
	eViewType_Role,						//地图主角
	eViewType_Npc,						//地图怪物
	eViewType_Max		
};

class CCmptView;
class CSceneMap;
class CSceneScreen : public zScreenIndex
{
public:
	using CmptViewSet = std::set<CCmptView*, std::less<CCmptView*>>;
	using CmptViewVec = std::vector<CCmptView*>;
	using PosISetMap = std::map<zPosI, CmptViewSet, std::less<zPosI>>;
	using PosIVecMap = std::map<zPosI, CmptViewVec, std::less<zPosI>>;
	using PosIRoleMap = std::map<zPosI, uint32, std::less<zPosI>>;
	using PosIMonsterMap = std::map<zPosI, uint32, std::less<zPosI>>;
public:
	static const zPosIVector& GetFullScreen();
	static const zPosIVector& GetEmptyScreen();
public:
	CSceneScreen();
	~CSceneScreen();
public:
	inline uint32 SizeView(eViewType eType) const { return m_arrViewCount[eType]; }
	inline uint32 
private:
	size_t m_arrViewCount[eViewType_Max];								//视野类型计数(整个地图视野角色数和怪物数)
	PosISetMap m_mapSetIndex[eViewType_Max];							//每屏角色和怪物set
	PosIVecMap m_mapVecIndex[eViewType_Max];							//每屏角色和怪物vec
	CmptViewSet m_setAll[eViewType_Max];								//整个地图视野角色和怪物
	PosIRoleMap m_mapRoleCount;											//每屏主角计数
	PosIMonsterMap m_mapMonsterCount;									//每屏怪物计数
};