#include "innermatch.pb.h"
#include "innercommon.pb.h"
#include "matchcommon.pb.h"

#include "MatchMem.h"
#include "MatchUnit.h"
#include "RobotConfig.h"

MatchMem::MatchMem(zRoleIdType roleId, const inner::InnerMatchMatcher& in) :
	roleId(roleId)
{
	_info = new inner::InnerMatchMatcher(in);
}

MatchMem::~MatchMem()
{
	assert(unit == nullptr);
	SAFE_DELETE(_info);
}

void MatchMem::final()
{
}

void MatchMem::fill(common::RoleBrief& out)
{
	out = _info->brief();
}

void MatchMem::fill(inner::InnerMatchFighter& out)
{
	fill(*out.mutable_brief());
	fill(*out.mutable_extd());
	out.set_localleaderid(getLocalLeaderId());
}

void MatchMem::fill(inner::InnerCopyTeamMem& out)
{
	assert(unit != nullptr);
	out.set_roleid(getRoleId());
	out.set_zoneidf(unit->getZoneId());
}

zRoleIdType MatchMem::getLocalLeaderId()
{
	return _info->localleaderid();
}

const std::string& MatchMem::getNickName()
{
	return _info->brief().nickname();
}

uint32 MatchMem::getCareerId()
{
	return _info->brief().careerid();
}

uint32 MatchMem::getLevel()
{
	return _info->brief().level();
}

bool MatchMem::canConfirm()
{
	if (unit == nullptr) return false;
	//if (unit->room == nullptr) return false;
	if (isConfirmed()) return false;
	//return unit->room->isTimeForConfirm();
	return true;
}

bool MatchMem::needPunish()
{
	return isConfirmedRefuse() || isConfirmedTimeout() || isConfiremdForceCancel();
}

bool MatchMem::isConfirmed()
{
	return _confirm != cMatch::eConfirmNone;
}

bool MatchMem::isConfirmedOk()
{
	return _confirm == cMatch::eConfirmOk;
}

bool MatchMem::isConfirmedRefuse()
{
	return _confirm == cMatch::eConfirmRefuse;
}

bool MatchMem::isConfirmedTimeout()
{
	return _confirm == cMatch::eConfirmTimeout;
}

bool MatchMem::isConfiremdForceCancel()
{
	return _confirm == cMatch::eConfirmForceCancel;
}

void MatchMem::setConfirmOk()
{
	assert(unit != nullptr);
	//assert(unit->room != nullptr);
	_confirm = cMatch::eConfirmOk;
	/*
	* NetInterface::get().sendCmdTeamSvrLocal(unit->zoneRef().getZoneId(),[&](auto& out)->bool
	{
		out.set_fromuser(getRoleId());
		auto& ntf = *out.mutable_frommatch_roleconfirmok();
		ntf.set_roomid(unit->room->getRoomId());
	});
	*/
}

void MatchMem::setConfirmRefuse()
{
	assert(unit != nullptr);
	//assert(unit->room != nullptr);
	_confirm = cMatch::eConfirmRefuse;
	///unit->room->setConfirmRefuse();
}

void MatchMem::setConfirmTimeout()
{
	assert(unit != nullptr);
	//assert(unit->room != nullptr);
	_confirm = cMatch::eConfirmTimeout;
}

void MatchMem::setConfirmForceCancel()
{
	assert(unit != nullptr);
	//assert(unit->room != nullptr);
	_confirm = cMatch::eConfirmForceCancel;
}

void MatchMem::resetConfirm()
{
	assert(unit != nullptr);
	//assert(unit->room != nullptr);
	_confirm = cMatch::eConfirmNone;
}

MemRobot::MemRobot(zRoleIdType robotId, const inner::InnerMatchMatcher& in, const config::robot_info_t& cfg) :
	MatchMem(robotId, in),
	robotTid(cfg.tid)
{
}

MemRobot::~MemRobot()
{
}

MemRole::MemRole(zRoleIdType roleId, const inner::InnerMatchMatcher& in) :
	MatchMem(roleId, in)
{
	joinTime = GetCurrTime();
}

MemRole::~MemRole()
{
}