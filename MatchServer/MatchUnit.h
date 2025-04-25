#pragma once

#include "cMatch.h"
#include "MatchDefine.h"

namespace common
{
	class MatchStatus;
}

namespace inner
{
	class InnerMatchUnit;
}

class MatchMem;
class MemRole;
class MemRobot;
class MatchQueue;
class MatchQueueMgr;
class MatchRoom;
class MatchUnit
{
public:
	using FuncMemberT = std::function<bool(MatchMem&)>;
	using FuncRoleT = std::function<bool(MemRole&)>;
	using FuncRobotT = std::function<bool(MemRobot&)>;
	using QueueMapT = std::map<uint64, MatchQueue*>;
	using MemberListT = std::list<MatchMem*>;
	using RoleListT = std::list<MemRole*>;
	using RobotListT = std::list<MemRobot*>;
public:
	MatchUnit(const uint32 zoneId, zUnitIdType uintId, MatchQueueMgr& mgr);
	virtual ~MatchUnit();
	virtual void final();
	virtual void addInitQueue(bool front) = 0;
	virtual void addMemRole(MemRole* pRole);
	virtual void addMemRobot(MemRobot* pRobot);
public:
	MatchQueueMgr& mgrRef() { return _mgr; }
	uint32 getZoneId() const { return _zoneId; }
	zUnitIdType getUnitId() const { return _unitId; }
	uint64 getCreateTime() const { return _createTime; }
	size_t sizeMember() { return _members.size(); }
	size_t sizeRole() { return _roles.size(); }
	size_t sizeRobot() { return _robots.size(); }
	size_t sizeQueue() { return _queues.size(); }
	bool emptyMember() { return _members.empty(); }
	bool emptyRole() { return _roles.empty(); }
	bool emptyRobot() { return _robots.empty(); }
	bool needPunish();
	uint64 getMatchTime();
public:
	bool foreach(FuncMemberT func);
	bool foreachRole(FuncRoleT func);
	bool foreachRobot(FuncRobotT func);
	void addToQueue(MatchQueue* queue);
	void removeFromQueue(MatchQueue* queue);
	void removeFromAllQueue();
	void removeFromRoom();
public:
	void fill(common::MatchStatus& out);
	void fill(inner::InnerMatchUnit& out);
	void forceBreakNtf();
	void forceBreak();
	void startMatch();
	void restartMatch();
	void restartMatchFromQueue();
	void cancelMatch();
	void punishMatch();
	void finalMe();
private:
	void __removeRobot(MemRobot* pRobot);
	void __removeAllRobot();
	void __removeRole(MemRole* pRole);
	void __removeAllRole();
public:
	MatchRoom* room = nullptr;
protected:
	QueueMapT _queues;									//当前匹配单元在哪些匹配队列中
private:
	uint32 _zoneId = 0;
	MatchQueueMgr& _mgr;
	zUnitIdType _unitId = 0;
	MemberListT _members;
	RoleListT _roles;
	RobotListT _robots;
	uint64 _createTime = 0;
};