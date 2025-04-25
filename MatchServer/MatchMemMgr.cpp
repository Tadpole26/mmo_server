#include "innermatch.pb.h"

#include "cGame.h"

#include "MatchMem.h"
#include "MemRoleF55.h"
#include "MemRobotF55.h"
#include "MatchMemMgr.h"
#include "MatchQueueMgr.h"
#include "RobotConfig.h"

MatchMemMgr::MatchMemMgr()
{
}

MatchMemMgr::~MatchMemMgr()
{
}

void MatchMemMgr::final()
{
	__recycle();
}

void MatchMemMgr::tick()
{
	__recycle();
}

MemRole* MatchMemMgr::createRole(MatchQueueMgr& mgr, zRoleIdType roleId, const inner::InnerMatchMatcher& in)
{
	MemRole* pMem = nullptr;
	switch (mgr.getMtype())
	{
	case cMatch::eMatchF55: pMem = new MemRoleF55(roleId, in);		break;
	default: assert(false);											break;
	}
	if (pMem == nullptr)
	{
		Log_Error("createRole.pMem==nullptr,%lu", roleId);
		return nullptr;
	}
	if (!_allroles.emplace(pMem->roleId, pMem).second)
	{
		Log_Error("createRole.!emplace,%lu", roleId);
		__addRecycle(pMem);
		return nullptr;
	}
	Log_Info("createRole,%lu", roleId);
	return pMem;
}

MemRobot* MatchMemMgr::createRobot(uint32 zoneId, MatchQueueMgr& mgr, uint32 cfgId, const inner::InnerMatchMatcher& in)
{
	auto* pCfg = gRobotCfg->getRobotConfig(cfgId);
	if (pCfg == nullptr)
	{
		Log_Error("createRobot.!pCfg,%lu", cfgId);
		return nullptr;
	}
	return createRobot(zoneId, mgr, *pCfg, in);
}

MemRobot* MatchMemMgr::createRobot(uint32 zoneId, MatchQueueMgr& mgr, const config::robot_info_t& cfg, const inner::InnerMatchMatcher& in)
{
	auto robotId = cGame::unionRobotId(zoneId, __getRobotAutoId());
	MemRobot* pMem = nullptr;
	switch (mgr.getMtype())
	{
	case cMatch::eMatchF55: pMem = new MemRobotF55(robotId, in, cfg);		break;
	default:	assert(false);												break;
	}
	if (pMem == nullptr)
	{
		Log_Error("createRobot.pMem==nullptr,roleId:%lu,cfgId:%u", robotId, cfg.tid);
		return nullptr;
	}
	if (!_allrobots.emplace(pMem->roleId, pMem).second)
	{
		Log_Error("createRobot.!emplace,roleId:%lu,cfgId:%u", robotId, cfg.tid);
		__addRecycle(pMem);
		return nullptr;
	}
	Log_Info("createRobot,roleId:%lu,cfgId:%u", robotId, cfg.tid);
	return pMem;
}

MemRole* MatchMemMgr::getRole(zRoleIdType roleId)
{
	auto iter = _allroles.find(roleId);
	if (iter != _allroles.end())
	{
		return iter->second;
	}
	return nullptr;
}

MemRobot* MatchMemMgr::getRobot(zRoleIdType robotId)
{
	auto iter = _allrobots.find(robotId);
	if (iter != _allrobots.end())
	{
		return iter->second;
	}
	return nullptr;
}

void MatchMemMgr::addRoleToRecycle(MemRole* pRole)
{
	assert(pRole != nullptr);
	assert(pRole->unit == nullptr);
	__removeRole(pRole);
	__addRecycle(pRole);
}

void MatchMemMgr::addRobotToRecycle(MemRobot* pRobot)
{
	assert(pRobot != nullptr);
	assert(pRobot->unit == nullptr);
	__removeRobot(pRobot);
	__addRecycle(pRobot);
}

void MatchMemMgr::__removeRole(MemRole* pRole)
{
	auto iter = _allroles.find(pRole->roleId);
	if (iter != _allroles.end())
	{
		_allroles.erase(iter);
	}
}

void MatchMemMgr::__removeRobot(MemRobot* pRobot)
{
	auto iter = _allrobots.find(pRobot->roleId);
	if (iter != _allrobots.end())
	{
		_allrobots.erase(iter);
	}
}

void MatchMemMgr::__addRecycle(MatchMem* pMem)
{
	_recycles.push_back(pMem);
}

void MatchMemMgr::__recycle()
{
	for (auto* pMem : _recycles)
	{
		pMem->final();
		SAFE_DELETE(pMem);
	}
	_recycles.clear();
}