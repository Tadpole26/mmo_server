#include "common.pb.h"
#include "fight3v3common.pb.h"
#include "fight5v5common.pb.h"
#include "team.pb.h"

#include "cScene.h"
#include "TeamUser.h"
#include "TeamMember.h"
#include "TeamLeader.h"
#include "SessionTeam.h"
#include "TeamUserMgr.h"
#include "ParamConfig.h"
#include "TeamConfig.h"

TeamUser::TeamUser(const zRoleIdType roleId) :
	cUser(roleId)
{
	_brief = new common::RoleBrief();
	_extd1 = new common::RoleExtend1();
	_extd2 = new common::RoleExtend2();
	_extd3 = new common::RoleExtend3();
	_f33 = new common::ExtendFight3v3();
	_f55 = new common::ExtendFight5v5();
}

TeamUser::~TeamUser()
{
}

void TeamUser::final()
{
	gTeamUserMgr->removeRole(this);
	SAFE_DELETE(_brief);
	SAFE_DELETE(_extd1);
	SAFE_DELETE(_extd2);
	SAFE_DELETE(_extd3);
	SAFE_DELETE(_f33);
	SAFE_DELETE(_f55);
	cUser::final();
}

void TeamUser::setSceneHashId(uint32 hashId)
{
	if (_sceneHashId != hashId)
	{
		_sceneHashId = hashId;
		Log_Debug("setSceneHashId.%lu.%lu", _sceneHashId, hashId);
	}
}

void TeamUser::fill(common::RoleBrief& out)
{
	out = *_brief;
}

void TeamUser::fill(common::RoleExtend1& out)
{
	out = *_extd1;
}

void TeamUser::fill(common::RoleExtend2& out)
{
	out = *_extd2;
}

void TeamUser::fill(common::RoleExtend3& out)
{
	out = *_extd3;
}

void TeamUser::fill(common::ExtendFight3v3& out)
{
	out = *_f33;
}

void TeamUser::fill(common::ExtendFight5v5& out)
{
	out = *_f55;
}

void TeamUser::fill(common::NotOwnTeamInfo& out)
{
}

void TeamUser::fill(common::OwnTeamInfo& out)
{
}

void TeamUser::fill(common::TeamInvited& out)
{
	out.set_roleid(_inviteUser->getRoleId());
	out.set_nickname(_inviteUser->getNickName());
	out.set_expire(_inviteExpire);
	out.set_teamid(_inviteTeamId);
	out.set_leadersceneid(_inviteLeaderSceneId);
}

void TeamUser::fill(common::TeamMemberFieldList& out)
{

}

void TeamUser::setBrief(const common::RoleBrief& in)
{
	*_brief = in;
	setNickName(_brief->nickname());
}

void TeamUser::setExtd1(const common::RoleExtend1& in)
{
	*_extd1 = in;
}

void TeamUser::setExtd2(const common::RoleExtend2& in)
{
	*_extd2 = in;
}

void TeamUser::setExtd3(const common::RoleExtend3& in)
{
	*_extd3 = in;
}

void TeamUser::setF33(const common::ExtendFight3v3& in)
{
	*_f33 = in;
}

void TeamUser::setF55(const common::ExtendFight5v5& in)
{
	*_f55 = in;
}

const std::string TeamUser::getMapInfoByte(){  return _extd2->sceneid().SerializeAsString(); }
const std::string TeamUser::getMapPosByte() { return _extd2->cpos().SerializeAsString(); }
const std::string TeamUser::getModelByte() { return _extd1->mplist().SerializeAsString(); }
const std::string TeamUser::getFashionByte() { return _extd1->mplist().SerializeAsString(); }
const std::string TeamUser::getMainHandByte() { return _extd1->mainhand().SerializeAsString(); }
const std::string TeamUser::getOffHandByte() { return _extd1->offhand().SerializeAsString(); }
uint32 TeamUser::getHeadIcon() { return _brief->headicon(); }
uint32 TeamUser::getHeadFrame() { return _brief->headframe(); }
uint32 TeamUser::getCareerId() { return _brief->careerid(); }
uint32 TeamUser::getLevel() { return _brief->level(); }
uint32 TeamUser::getCareerLevel() { return _brief->careerid(); }
uint32 TeamUser::getHp() { return _extd3->hp(); }
uint32 TeamUser::getMp() { return _extd3->mp(); }
uint32 TeamUser::getMaxHp() { return _extd3->maxhp(); }
uint32 TeamUser::getMaxMp() { return _extd3->maxmp(); }
uint32 TeamUser::getSex() { return _brief->sex(); }
uint32 TeamUser::getFightPoint() { return _extd1->fightpoint(); }
uint32 TeamUser::getChatFont() { return _extd1->chatfont(); }
uint32 TeamUser::getChatFrame() { return _extd1->chatframe(); }
zSceneIdType TeamUser::getSceneId()
{
	const auto sceneId = _extd2->sceneid();
	//cScene::UnionSceneId usi(sceneId);
	return 0;
}

void TeamUser::resetInvite()
{
	_inviteUser = nullptr;
	_inviteExpire = 0;
	_inviteTeamId = 0;
	_inviteLeaderSceneId = 0;
}

void TeamUser::setInvite(TeamUser* pUser)
{
	if (pUser == this) return;
	auto& team = pUser->pMem->teamRef();
	_inviteUser = pUser;
	_inviteExpire = GetCurrTime() + gParamCfg->teamInviteExpireSec;
	_inviteTeamId = team.getTeamId();
	_inviteLeaderSceneId = team.teamLeaderRef().getSceneId();
	client::ModuleTeam_Ntf_AddInvited ntf;
	fill(*ntf.mutable_invited());
	//sendCmdToMe();
}

bool TeamUser::isExpireInvite()
{
	return _inviteExpire <= GetCurrTime();
}

void TeamUser::resetExpireApply()
{
	_apply = 0;
}

void TeamUser::setExpireApply()
{
	_apply = GetCurrTime() + gParamCfg->teamApplyColdSec;
}

bool TeamUser::isExpireApply()
{
	return  _apply <= GetCurrTime();
}

void TeamUser::resetExpireCreate()
{
	_create = 0;
}

void TeamUser::setExpireCreate()
{
	_create = GetCurrTime() + gParamCfg->teamCreateColdSec;
}

bool TeamUser::isExpireCreate()
{
	return  _create <= GetCurrTime();
}

void TeamUser::tryExpireRemove()
{
	if (isExpireApply()) resetExpireApply();
	if (isExpireCreate()) resetExpireCreate();
	if (isExpireInvite()) resetInvite();
	//if (isExpireTotal()) 
}

void TeamUser::setVotePtr(TeamVote* vote)
{
	assert((bool)vote == !isVoting());
	_vote = vote;
}
