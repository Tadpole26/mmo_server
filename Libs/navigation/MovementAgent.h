#pragma once
#include "sdk.h"

enum PolyFlags
{
	POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
	POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
	POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
	POLYFLAGS_JUMP = 0x08,		// Ability to jump.
	POLYFLAGS_DISABLED = 0x10,	// Disabled polygon
	POLYFLAGS_ALL = 0xffff		// All abilities.
};


class NavMeshContext;
class dtQueryFilter;
class MovementAgent
{
public:
	MovementAgent();
	virtual ~MovementAgent();
	virtual void final();
public:
	virtual void getPos(float &x, float &y, float &z) const = 0;			//当前自己坐标
	virtual const NavMeshContext* getNavMeshContext() const = 0;			//当前地图navmesh
	virtual bool canWalk() const = 0;	
	dtQueryFilter& refQueryFilter();
private:
	dtQueryFilter* _filter = nullptr;
};