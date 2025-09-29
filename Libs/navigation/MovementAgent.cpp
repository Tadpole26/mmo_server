#include "MovementAgent.h"

#include "Detour/Include/DetourNavMeshQuery.h"

MovementAgent::MovementAgent()
{
	_filter = new dtQueryFilter();
	_filter->setIncludeFlags(POLYFLAGS_ALL ^ POLYFLAGS_DISABLED);
	_filter->setExcludeFlags(0);
	//filter.setIncludeFlags(POLYFLAGS_WALK);	// 仅允许地面行走
	//filter->setExcludeFlags(POLYFLAGS_SWIM);       // 排除水域
	//filter->setAreaCost(GRASS, 1.0f);            // 草地成本为1
	//filter->setAreaCost(SWAMP, 3.0f);            // 沼泽成本为3
}

MovementAgent::~MovementAgent()
{
}

void MovementAgent::final()
{
	SAFE_DELETE(_filter);
}

dtQueryFilter& MovementAgent::refQueryFilter()
{
	return *_filter;
}