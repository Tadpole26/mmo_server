#include "MovementAgent.h"

#include "Detour/Include/DetourNavMeshQuery.h"

MovementAgent::MovementAgent()
{
	_filter = new dtQueryFilter();
	_filter->setIncludeFlags(POLYFLAGS_ALL ^ POLYFLAGS_DISABLED);
	_filter->setExcludeFlags(0);
	//filter.setIncludeFlags(POLYFLAGS_WALK);	// �������������
	//filter->setExcludeFlags(POLYFLAGS_SWIM);       // �ų�ˮ��
	//filter->setAreaCost(GRASS, 1.0f);            // �ݵسɱ�Ϊ1
	//filter->setAreaCost(SWAMP, 3.0f);            // ����ɱ�Ϊ3
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