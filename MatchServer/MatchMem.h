#pragma once

#include "cMatch.h"
#include "zRoleMgr.h"

namespace config
{
	struct robot_info_t;
}

namespace common
{
	class RoleBrief;
	class MatchFighterExtend;
}

namespace inner
{
	class InnerMatchMatcher;
	class InnerMatchFighter;
	class InnerCopyTeamMem;
}

//匹配玩家
class MatchUnit;
class MatchMem
{
private:
	uint32 _confirm = cMatch::eConfirmNone;
protected:
	inner::InnerMatchMatcher* _info = nullptr; //匹配信息
public:
	MatchUnit* unit = nullptr;	//所在匹配单元
	zRoleIdType roleId = 0;
public:
	MatchMem(zRoleIdType roleId, const inner::InnerMatchMatcher& in);
	virtual ~MatchMem();
	virtual void final();
public:
	virtual bool isRobot() { return false; }
	virtual bool isRole() { return false; }
	virtual bool isReady() { return false; }
public:
	void fill(common::RoleBrief& out);
	void fill(inner::InnerMatchFighter& out);
	void fill(inner::InnerCopyTeamMem& out);
	virtual void fill(common::MatchFighterExtend& out) = 0;
public:
	zRoleIdType getRoleId() { return roleId; }
	zRoleIdType getLocalLeaderId();
	const std::string& getNickName();
	uint32 getCareerId();
	uint32 getLevel();
public:
	bool canConfirm();
	bool needPunish();
	bool isConfirmed();
	bool isConfirmedOk();
	bool isConfirmedRefuse();
	bool isConfirmedTimeout();
	bool isConfiremdForceCancel();
	void setConfirmOk();
	void setConfirmRefuse();
	void setConfirmTimeout();
	void setConfirmForceCancel();
	void resetConfirm();
};

//匹配机器人
class MemRobot : public MatchMem
{
public:
	MemRobot(zRoleIdType robotId, const inner::InnerMatchMatcher& in, const config::robot_info_t& cfg);
	virtual ~MemRobot();
	bool isRobot() override { return true; }
	bool isReady() override { return true; }
public:
	uint32 robotTid = 0;
};

class MemRole : public MatchMem
{
public:
	MemRole(zRoleIdType roleId, const inner::InnerMatchMatcher& in);
	~MemRole();
	bool isRole() override { return true; }
public:
	uint64 joinTime = 0;			//加入时间
};
