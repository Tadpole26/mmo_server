#pragma once

#include "singleton.h"
#include "sdk.h"

namespace inner
{
	class InnerMatchMatcher;
}

namespace config
{
	struct robot_info_t;
}

class MatchMem;
class MemRole;
class MemRobot;
class MatchQueueMgr;
class MatchMemMgr
{
public:
	using RoleMapT = std::unordered_map<zRoleIdType, MemRole*>;
	using RobotMapT = std::unordered_map<zRoleIdType, MemRobot*>;
	using MemberListT = std::list<MatchMem* >;
public:
	MatchMemMgr();
	~MatchMemMgr();
	void final();
	void tick();
public:
	size_t sizeRole() { return _allroles.size(); }
	size_t sizeRobot() { return _allrobots.size(); }
	size_t sizeRecycle() { return _recycles.size(); }
	MemRole* createRole(MatchQueueMgr& mgr, zRoleIdType roleId, const inner::InnerMatchMatcher& in);
	MemRobot* createRobot(uint32 zoneId, MatchQueueMgr& mgr, uint32 cfgId, const inner::InnerMatchMatcher& in);
	MemRobot* createRobot(uint32 zoneId, MatchQueueMgr& mgr, const config::robot_info_t& cfg, const inner::InnerMatchMatcher& in);
	MemRole* getRole(zRoleIdType roleId);
	MemRobot* getRobot(zRoleIdType robotId);
	void addRoleToRecycle(MemRole* pRole);
	void addRobotToRecycle(MemRobot* pRobot);
private:
	uint64 __getRobotAutoId() { return ++_robotAutoId; }
	void __removeRole(MemRole* pRole);
	void __removeRobot(MemRobot* pRobot);
	void __addRecycle(MatchMem* pMem);
	void __recycle();
private:
	uint64 _robotAutoId = 0;
	RoleMapT _allroles;
	RobotMapT _allrobots;
	MemberListT _recycles;
};

#define gMatchMemMgr Singleton<MatchMemMgr>::getInstance()