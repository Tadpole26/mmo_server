
#include "innermatch.pb.h"
#include "matchcommon.pb.h"

#include "zLogMgr.h"

#include "MatchMem.h"
#include "MatchUnit.h"
#include "MatchQueue.h"
#include "MatchRoom.h"
#include "MatchUnitMgr.h"
#include "MatchQueueMgr.h"

MatchUnit::MatchUnit(const uint32 zoneId, zUnitIdType uintId, MatchQueueMgr& mgr) :
	_zoneId(zoneId), _unitId(uintId), _mgr(mgr)
{
}

MatchUnit::~MatchUnit()
{
	_createTime = GetCurrTime();
}

void MatchUnit::final()
{
	assert(_members.empty());
	assert(_roles.empty());
	assert(_robots.empty());
}

void MatchUnit::addMemRole(MemRole* pRole)
{
	assert(pRole != nullptr);
	assert(pRole->unit == nullptr);
	_members.push_back(pRole);
	_roles.push_back(pRole);
	pRole->unit = this;
	Log_Info("addMemRole,%lu", pRole->getRoleId());
}

void MatchUnit::addMemRobot(MemRobot* pRobot)
{
	assert(pRobot != nullptr);
	assert(pRobot->unit == nullptr);
	_members.push_back(pRobot);
	pRobot->unit = this;
	Log_Info("addMemRobot,%lu", pRobot->getRoleId());
}

bool MatchUnit::foreach(FuncMemberT func)
{
	for (auto iter = _members.begin(); iter != _members.end();)
	{
		auto del = iter++;
		auto* pMem = *del;
		if (!func(*pMem)) return false;
	}
	return true;
}

bool MatchUnit::foreachRole(FuncRoleT func)
{
	for (auto iter = _roles.begin(); iter != _roles.end();)
	{
		auto del = iter++;
		auto* pRole = *del;
		if (!func(*pRole)) return false;
	}
	return true;
}

bool MatchUnit::foreachRobot(FuncRobotT func)
{
	for (auto iter = _robots.begin(); iter != _robots.end();)
	{
		auto del = iter++;
		auto* pRobot = *del;
		if (!func(*pRobot)) return false;
	}
	return true;
}

void MatchUnit::addToQueue(MatchQueue* queue)
{
	assert(queue != nullptr);
	assert(_queues[queue->getEid()] == nullptr);
	_queues[queue->getEid()] = queue;
}

void MatchUnit::removeFromQueue(MatchQueue* queue)
{
	assert(queue != nullptr);
	_queues.erase(queue->getEid());
}

void MatchUnit::removeFromAllQueue()
{
	for (auto iter = _queues.begin(); iter != _queues.end();)
	{
		auto del = iter++;
		auto* queue = del->second;
		queue->removeUnit(this);
	}
}

void MatchUnit::__removeRobot(MemRobot* pRobot)
{
	assert(pRobot != nullptr);
	assert(pRobot->unit == this);
	for (auto iter = _members.begin(); iter != _members.end();)
	{
		auto del = iter++;
		auto* pMem = *del;
		if (pRobot->roleId == pMem->roleId)
		{
			_members.erase(del);
			break;
		}
	}
	for (auto iter = _robots.begin(); iter != _robots.end();)
	{
		auto del = iter++;
		auto* pMem = *del;
		if (pRobot->roleId == pMem->roleId)
		{
			_robots.erase(del);
			break;
		}
	}
	pRobot->unit = nullptr;
	//MatchMemMgr::get().addRobotToRecycle(pRobot);
}

void MatchUnit::__removeAllRobot()
{
	for (auto iter = _robots.begin(); iter != _robots.end();)
	{
		auto del = iter++;
		auto* pRobot = *del;
		__removeRobot(pRobot);
	}
}

void MatchUnit::__removeRole(MemRole* pRole)
{
	assert(pRole != nullptr);
	assert(pRole->unit == this);
	for (auto iter = _members.begin(); iter != _members.end();)
	{
		auto del = iter++;
		auto* pMem = *del;
		if (pRole->roleId == pMem->roleId)
		{
			_members.erase(del);
			break;
		}
	}
	for (auto iter = _roles.begin(); iter != _roles.end();)
	{
		auto del = iter++;
		auto* pMem = *del;
		if (pRole->roleId == pMem->roleId)
		{
			_roles.erase(del);
			break;
		}
	}
	pRole->unit = nullptr;
	//MatchMemMgr::get().addRobotToRecycle(pRole);
}

void MatchUnit::__removeAllRole()
{
	for (auto iter = _roles.begin(); iter != _roles.end();)
	{
		auto del = iter++;
		auto* pRole = *del;
		__removeRole(pRole);
	}
}

void MatchUnit::removeFromRoom()
{
	auto* pRoom = this->room;
	if (pRoom != nullptr)
	{
		pRoom->removeUnit(this);
		//this->room 已被设置位nullptr
	}
}

bool MatchUnit::needPunish()
{
	return !foreach([&](auto& member)->bool
	{
			return !member.needPunish();
	});
}

uint64 MatchUnit::getMatchTime()
{
	return GetCurrTime() - getCreateTime();
}

void MatchUnit::fill(common::MatchStatus& out)
{
	out.set_mtype(mgrRef().getMtype());
	out.set_mtime((uint32)getCreateTime());
}

void MatchUnit::fill(inner::InnerMatchUnit& out)
{
	foreachRole([&](auto& role)->bool
	{
		role.fill((*out.mutable_roles())[role.getRoleId()]);
		return true;
	});
	foreachRobot([&](auto& robot)->bool 
	{
		robot.fill((*out.mutable_robots())[robot.getRoleId()]);
		return true;
	});
}

void MatchUnit::forceBreakNtf()
{
	Log_Info("forceBreakNtf");
	/*
	NetInterface::get().sendCmdTeamsvrLocal(zoneRef().getZoneId(),[&](auto& out)
	{
		auto& ntf = *out.mutable_frommatch_cancelmatch();
		ntf.set_mtype(mgrRef().getMtype());
		ntf.set_unitid(getUnitId());
	});
	*/
	finalMe();
}

void MatchUnit::forceBreak()
{
	Log_Info("forceBreak");
	finalMe();
}

void MatchUnit::startMatch()
{
	Log_Info("startMatch");
	addInitQueue(false);
	/*
	NetInterface::get().sendCmdTeamsvrLocal(getZoneId(), [&](auto& out)
	{
		auto& ntf = *out.mutable_frommatch_startmatch();
		ntf.set_mtype(mgrRef().getMtype());
		ntf.set_unitid(getUnitId());
		auto& mlist = *ntf.mutable_matchlist();
		foreachRole([&](auto& role) 
		{
			mlist.add_roleids(role.getRoleId());
			return true;
		});
	});
	*/
}

void MatchUnit::restartMatch()
{
	Log_Info("startMatch");
	foreachRole([&](auto& role)->bool
	{
		role.resetConfirm();
		return true;
	});
	removeFromRoom();
	__removeAllRobot();
	if (!emptyRole())
	{
		addInitQueue(false);
		/*
		* NetInterface::get().sendCmdTeamsvrLocal(getZoneid(), [&](auto& out)
		{
			auto& ntf = *out.mutable_frommatch_restartmatch();
			ntf.set_unitid(getUnitId());
			ntf.set_type(cMatch::eRestartMatchFromRoom);
		});
		*/
	}
	else
		gMatchUnitMgr->addToRecycle(this);
}

void MatchUnit::restartMatchFromQueue()
{
	Log_Info("startMatch");
	removeFromAllQueue();
	__removeAllRobot();
	assert(!emptyRole());
	addInitQueue(true);
	/*
	* NetInterface::get().sendCmdTeamsvrLocal(getZoneId(), [&](auto& out) 
	{
		auto& ntf = *out.mutable_frommatch_restartmatch();
		ntf.set_unitid(getUnitId());
		ntf.set_type(cMatch::eRestartMatchFromQueue);
	});
	*/
}

void MatchUnit::cancelMatch()
{
	Log_Info("startMatch");
	/*
	NetInterface::get().sendCmdTeamsvrLocal(getZoneId(), [&](auto& out)
	{
		auto& ntf = *out.mutable_frommatch_cancelmatch();
		ntf.set_mtype(mgrRef().getMtype());
		ntf.set_unitid(getUnitId());
	});
	*/
	finalMe();
}

void MatchUnit::punishMatch()
{
	Log_Info("startMatch");
	/*
	* NetInterface::get().sendCmdTeamsvrLocal(getZoneId(), [&](auto& out)
	{
		auto& ntf = *out.mutable_frommatch_punishmatch();
		ntf.set_unitid(getUnitId());
		auto& plist = *ntf.mutable_punishlist();
		foreachRole([&](auto& role) 
		{
			if (role.needPunish())
			{
				plist.add_roleids(role.getRoleId());
			}
			return true;
		});
	});
	*/
	finalMe();
}

void MatchUnit::finalMe()
{
	Log_Info("finalMe");
	removeFromRoom();
	removeFromAllQueue();
	__removeAllRobot();
	__removeAllRole();
	gMatchUnitMgr->addToRecycle(this);
}