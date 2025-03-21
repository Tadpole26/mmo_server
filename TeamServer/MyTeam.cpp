#include "teamcommon.pb.h"
#include "team.pb.h"
#include "innerteam.pb.h"

#include "TeamApply.h"
#include "TeamUser.h"
#include "TeamUserMgr.h"
#include "TeamMember.h"
#include "TeamLeader.h"
#include "SessionTeam.h"
#include "TeamMgr.h"
#include "TeamConfig.h"

bool TeamUser::isTeamPersonMatching()
{
	return lvQueuePair.first != nullptr && getMatchTime() != 0;
}

void TeamUser::setMatchTime()
{
	_match = GetCurrTime();
}

void TeamUser::resetMatchTime()
{
	_match = 0;
}

void TeamUser::addToLvQueue(const config::team_info_t& cfg)
{
	assert(lvQueuePair.first == nullptr);
	auto* pTgQueue = &gTeamMgr->tgQueueRef(cfg);
	auto* pLvQueue = pTgQueue->lvQueuePtr(getLevel());
	pLvQueue->addMatchUser(this);
	Log_Info("use add to lv queue,targetId:%u,levelId:%u,size:%u", pTgQueue->getTargetId(), pLvQueue->getKey(), pLvQueue->sizeUser());
}

void TeamUser::removeFromLvQueue()
{
	assert(lvQueuePair.first != nullptr);
	auto* pLvQueue = lvQueuePair.first;
	auto* pTgQueue = pLvQueue->pTgQueue;
	pLvQueue->removeMatchUser(this);
	Log_Info("use remove from lv queue,targetId:%u,levelId:%u,size:%u", pTgQueue->getTargetId(), pLvQueue->getKey(), pLvQueue->sizeUser());
}

void TeamUser::mountTeam()
{
	client::ModuleTeam_Ntf_MountTeam ntf;
	auto& t = *ntf.mutable_team();
	if (pMem != nullptr)
	{
		pMem->teamRef().fill(*t.mutable_base());
		pMem->teamRef().fill(*t.mutable_memberlist());
		fill(*t.mutable_ownteam());
		if (pMem->isLeader())
			pMem->teamRef().teamLeaderRef().fill(*t.mutable_applylist());
		pMem->setOnline();
		pMem->broadChgFieldToScene({ inner::TMFType_State });
		pMem->sendChgFieldToTeam({ common::TMFieldType_State });
	}
	else
	{
		fill(*t.mutable_notteam());
	}
	//sendCmdToMe(client::enClientFirst_Team, client::eSecondTeam_Ntf_MoutTeam, ntf);
}

void TeamUser::leaveTeam(uint32 eLeave)
{
	assert(pMem != nullptr);
	tryExpireRemove();
	auto& team = pMem->teamRef();
	team.teamLeaderRef().tryExpireRemove();
	if (eLeave == common::enLeaveTeamType_Fire)				//解雇队员
	{
		assert(!pMem->isLeader());
		team.fireMember(pMem);
	}
	else if (!pMem->isLeader())								//队员离开	
	{
		team.leaveMember(pMem);
	}
	else                                                    //队长离开
	{
		TeamMember* pNextLeader = nullptr;
		team.foreachMem([&](auto& memRef)->bool
		{
			if (memRef.isLeader()) return true;
			if (pNextLeader == nullptr) pNextLeader = &memRef;
			else if (pNextLeader->getIndex() > memRef.getIndex()) pNextLeader = &memRef;
			return true;
		});
		if (pNextLeader != nullptr)
		{
			team.teamLeaderRef().appointLeader(pNextLeader);
			team.leaveMember(pMem);
		}
		else
		{
			team.leaveMemberLast();
			gTeamMgr->destroyTeam(team.getTeamId());
		}
	}
}

void TeamUser::startMatchPerson(const config::team_info_t& cfg)
{
	assert(pMem == nullptr);
	addToLvQueue(cfg);
	setMatchTime();
	/*
	NetInterface::get().broadCmdScenesvrAuto([&](auto& out)
	{
		out.set_fromuser(getRoleId());
		auto &ntf = *out.mutable_fromteam_startmatch();
		ntf.set_matchtime(getMatchTime());
	});
	*/
	//sendChgFieldToMe({common::TMFieldType_Match});
}

void TeamUser::cancelMatchPerson()
{
	removeFromLvQueue();
	resetMatchTime();
	/*
	NetInterface::get().broadCmdScenesvrAuto([&](auto& out)
	{
		out.set_fromuser(getRoleId());
		auto &ntf = *out.mutable_fromteam_cancelmatch();
		ntf.set_matchtime(getMatchTime());
	});
	*/
	//sendChgFieldToMe({common::TMFieldType_Match});
}

bool TeamUser::innerPersonChangeTarget(uint32 targetId)
{
	tryExpireRemove();
	if (pUnit != nullptr)
	{
		Log_Error("pvp is matching");
		return false;
	}
	if (pMem != nullptr)
	{
		Log_Error("self already has team");
		return false;
	}
	if (isTeamPersonMatching())
	{
		Log_Error("is matching team");
		return false;
	}
	auto* pCfg = gTeamCfg->getTeamConfig(targetId);
	if (!pCfg)
	{
		Log_Error("config in not exist,%u", targetId);
		return false;
	}
	if (getLevel() < pCfg->lvMin)
	{
		Log_Error("less target leve");
		return false;
	}
	if (getLevel() > pCfg->lvMax)
	{
		Log_Error("greater target level");
		return false;
	}
	startMatchPerson(*pCfg);
}

bool TeamUser::innerPersonCancelMatch()
{
	tryExpireRemove();
	if (pMem != nullptr)
	{
		Log_Error("self already has team");
		return false;
	}
	if (!isTeamPersonMatching())
	{
		Log_Error("is matching team");
		return false;
	}
	cancelMatchPerson();
}

bool TeamUser::innerLeaderStartMatch(const std::string& text, uint32 languageId, uint32 teamId)
{
	tryExpireRemove();
	if (pUnit != nullptr)
	{
		Log_Error("pvp is matching");
		return false;
	}
	if (pMem == nullptr)
	{
		Log_Error("self has no team");
		return false;
	}
	pMem->teamRef().teamLeaderRef().tryExpireRemove();
	if (!pMem->isLeader())
	{
		Log_Error("only leader op");
		return false;
	}
	auto* pTeam = pMem->teamPtr();
	if (pTeam->isMemberFull())
	{
		Log_Error("team already full");
		return false;
	}
	pTeam->setText(text);
	//pTeam->sendChgFieldToTeam({common::TFieldType_Text});
	if (!pTeam->isMatching())
	{
		pTeam->starMatch();
	}
	else
	{
		//pTeam->broadRecruitToWorld();
	}
}

bool TeamUser::innerLeaderCancelMatch()
{
	tryExpireRemove();
	if (pUnit != nullptr)
	{
		Log_Error("pvp is matching");
		return false;
	}
	if (pMem == nullptr)
	{
		Log_Error("self has no team");
		return false;
	}
	pMem->teamRef().teamLeaderRef().tryExpireRemove();
	if (!pMem->isLeader())
	{
		Log_Error("only leader op");
		return false;
	}
	auto* pTeam = pMem->teamPtr();
	if (!pTeam->isMatching())
	{
		Log_Error("team not matching");
		return false;
	}
	pTeam->cancelMatch();
}

bool TeamUser::innerCreateTeam(uint32 targetId, uint32 minLevel, uint32 maxLevel)
{
	tryExpireRemove();
	if (pMem != nullptr)
	{
		Log_Error("self in team");
		return false;
	}
	if (!isExpireCreate())
	{
		Log_Error("cold create");
		return false;
	}
	if (isTeamPersonMatching())
	{
		Log_Error("role matching");
		return false;
	}
	auto* pCfg = gTeamCfg->getTeamConfig(targetId);
	if (pCfg == nullptr)
	{
		Log_Error("not exist target id");
		return false;
	}
	if (minLevel < pCfg->lvMin)
	{
		Log_Error("less target min lv");
		return false;
	}
	if (minLevel > pCfg->lvMax)
	{
		Log_Error("greater target max lv");
		return false;
	}
	if (maxLevel < pCfg->lvMin)
	{
		Log_Error("less target min lv");
		return false;
	}
	if (maxLevel > pCfg->lvMax)
	{
		Log_Error("greater target max lv");
		return false;
	}
	auto* pTeam = gTeamMgr->createTeam();
	if (pTeam == nullptr)
	{
		Log_Error("create team fail");
		return false;
	}
	pTeam->setTarget(pCfg->tid);
	pTeam->setMinLevel(minLevel);
	pTeam->setMaxLevel(maxLevel);
	pTeam->createLeader(this);
	return true;
}

bool TeamUser::innerLeaderChangeTarget(uint32 targetId, uint32 minLevel, uint32 maxLevel)
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	pMem->teamRef().teamLeaderRef().tryExpireRemove();
	if (!pMem->isLeader())
	{
		return false;
	}
	auto* pCfg = gTeamCfg->getTeamConfig(targetId);
	if (pCfg == nullptr)
	{
		return false;
	}
	if (minLevel < pCfg->lvMin)
	{
		Log_Error("less target min lv");
		return false;
	}
	if (minLevel > pCfg->lvMax)
	{
		Log_Error("greater target max lv");
		return false;
	}
	if (maxLevel < pCfg->lvMin)
	{
		Log_Error("less target min lv");
		return false;
	}
	if (maxLevel > pCfg->lvMax)
	{
		Log_Error("greater target max lv");
		return false;
	}
	auto* pTeam = pMem->teamPtr();
	auto bmatch = pTeam->isMatching();
	if (pTeam->isMemberFull() && bmatch)
	{
		pTeam->teamLeaderRef().deleteApplyAll();
		pTeam->cancelMatch();
		pTeam->removeFromSzQueueTarget();
		pTeam->removeFromSzQueueLevelAll();

		pTeam->setTarget(pCfg->tid);
		pTeam->setMinLevel(minLevel);
		pTeam->setMaxLevel(maxLevel);
	}
	else if (pTeam->isMemberFull() && !bmatch)
	{
		pTeam->setTarget(pCfg->tid);
		pTeam->setMinLevel(minLevel);
		pTeam->setMaxLevel(maxLevel);
	}
	else if (bmatch)
	{
		pTeam->removeFromSzQueueTarget();
		pTeam->removeFromSzQueueLevelAll();

		pTeam->setTarget(pCfg->tid);
		pTeam->setMinLevel(minLevel);
		pTeam->setMaxLevel(maxLevel);

		pTeam->addToSzQueueTarget();
		pTeam->addToSzQueueLevelAll();
	}
	else
	{
		pTeam->removeFromSzQueueTarget();

		pTeam->setTarget(pCfg->tid);
		pTeam->setMinLevel(minLevel);
		pTeam->setMaxLevel(maxLevel);

		pTeam->addToSzQueueTarget();
	}
	//pTeam->broadChgFieldToScene({inner::TFType_TargetId, inner::TFType_MinLevel, inner::TFType_MaxLevel});
	//pTeam->sendChgFieldToTeam({common::TFieldType_TargetId, common::TFieldType_MinLevel, common::TFieldType_MaxLevel});
}

bool TeamUser::innerLeaderFireMember(zRoleIdType roleId)
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	pMem->teamRef().teamLeaderRef().tryExpireRemove();
	if (!pMem->isLeader())
	{
		return false;
	}
	if (getRoleId() == roleId)
	{
		return false;
	}
	auto* pFind = pMem->teamRef().getMember(roleId);
	if (pFind == nullptr)
	{
		return false;
	}
	gTeamMgr->batLeaveTeam({pFind->pUser}, common::enLeaveTeamType_Fire);
	return true;
}

bool TeamUser::innerLeaderTogetherMember(zRoleIdType roleId)
{
	tryExpireRemove();
	if (pMem == nullptr)
		return false;
	auto& team = pMem->teamRef();
	auto& teamLeader = team.teamLeaderRef();
	teamLeader.tryExpireRemove();
	if (!pMem->isLeader())
	{
		return false;
	}
	if (!pMem->isExpireTogether())
	{
		return false;
	}
	TeamMember* pMember = nullptr;
	if (roleId != 0)
	{
		pMember = team.getMember(roleId);
		if (pMember == nullptr)
		{
			return false;
		}
	}
	teamLeader.togetherMember(pMember);
}

bool TeamUser::innerleaderAppoint(zRoleIdType roleId)
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	pMem->teamRef().teamLeaderRef().tryExpireRemove();
	if (!pMem->isLeader())
	{
		return false;
	}
	if (getRoleId() == roleId)
	{
		return false;
	}
	if (!pMem->isExpireAppoint())
	{
		return false;
	}
	auto* pMember = pMem->teamRef().getMember(roleId);
	if (pMember == nullptr)
	{
		return false;
	}
	auto& teamLeader = pMem->teamRef().teamLeaderRef();
	teamLeader.appointLeader(pMember);
	return true;
}

bool TeamUser::innerLeaderRefuseAllApply()
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	auto& team = pMem->teamRef();
	team.teamLeaderRef().tryExpireRemove();
	if (!pMem->isLeader())
	{
		return false;
	}
	if (team.teamLeaderRef().applyEmpty())
	{
		return false;
	}
	team.teamLeaderRef().deleteApplyAll();
}

bool TeamUser::innerLeaderAgreeAllApply()
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	auto& team = pMem->teamRef();
	team.teamLeaderRef().tryExpireRemove();
	if (!pMem->isLeader())
	{
		return false;
	}
	if (team.teamLeaderRef().applyEmpty())
	{
		return false;
	}
	std::list<TeamUser*> roleList;
	team.teamLeaderRef().foreachApply([&](auto* apply)->bool
		{
			if (apply->isExpire()) return true;
			if (apply->pUser == nullptr) return true;
			if (apply->pUser->pMem != nullptr) return true;
			if (team.isMemberFull()) return false;
			roleList.push_back(apply->pUser);
			return true;
		});
	for (auto& pRole : roleList)
	{
		team.teamLeaderRef().agreeApply(pRole);
	}
	if (!team.isMemberFull())
	{
		team.teamLeaderRef().deleteApplyAll();
	}
	return true;
}

bool TeamUser::innerLeaderRefuseApply(uint32 zoneIdF, zRoleIdType roleId)
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	pMem->teamRef().teamLeaderRef().tryExpireRemove();
	if (!pMem->isLeader())
	{
		return false;
	}
	auto* pUserFind = gTeamUserMgr->tryCreateRole(zoneIdF, roleId);
	if (pUserFind == nullptr)
	{
		return false;
	}
	auto& teamLeader = pMem->teamRef().teamLeaderRef();
	if (!teamLeader.hasApply(pUserFind))
	{
		return false;
	}
	teamLeader.refuseApply(pUserFind);
	return true;
}

bool TeamUser::innerLeaderAgreeApply(uint32 zoneIdF, zRoleIdType roleId)
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	auto& team = pMem->teamRef();
	team.teamLeaderRef().tryExpireRemove();
	if (!pMem->isLeader())
	{
		return false;
	}
	auto* pUserFind = gTeamUserMgr->tryCreateRole(zoneIdF, roleId);
	if (pUserFind == nullptr)
	{
		return false;
	}
	if (pUserFind->pMem != nullptr)
	{
		return false;
	}
	if (!team.teamLeaderRef().hasApply(pUserFind))
	{
		return false;
	}
	if (!team.isMemberFull())
	{
		return false;
	}
	team.teamLeaderRef().agreeApply(pUserFind);
	return true;
}

bool TeamUser::innerInviteFriend(zRoleIdType roleId)
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	auto& team = pMem->teamRef();
	team.teamLeaderRef().tryExpireRemove();
	auto* pUserFind = gTeamUserMgr->tryCreateRole(getZoneIdF(), roleId);		//好友是本服的
	if (pUserFind == nullptr)
	{
		return false;
	}
	if (pUserFind->pMem != nullptr)
	{
		return false;
	}
	if (pUserFind->hasInvited())
	{
		return false;
	}
	pUserFind->setInvite(this);
	return true;
}

bool TeamUser::innerInviteFamily(zRoleIdType roleId)
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	auto& team = pMem->teamRef();
	team.teamLeaderRef().tryExpireRemove();
	auto* pUserFind = gTeamUserMgr->tryCreateRole(getZoneIdF(), roleId);		//好友是本服的
	if (pUserFind == nullptr)
	{
		return false;
	}
	if (pUserFind->pMem != nullptr)
	{
		return false;
	}
	if (pUserFind->hasInvited())
	{
		return false;
	}
	pUserFind->setInvite(this);
	return true;
}

bool TeamUser::innerInvitePlayer(uint32 zoneIdF, zRoleIdType roleId)
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	auto& team = pMem->teamRef();
	team.teamLeaderRef().tryExpireRemove();
	auto* pUserFind = gTeamUserMgr->tryCreateRole(getZoneIdF(), roleId);		//好友是本服的
	if (pUserFind == nullptr)
	{
		return false;
	}
	if (pUserFind->pMem != nullptr)
	{
		return false;
	}
	if (pUserFind->hasInvited())
	{
		return false;
	}
	/*
	* 
	*/
	pUserFind->setInvite(this);
	return true;
}

bool TeamUser::innerAgreeInvited(uint32 sceneHashId)
{
	tryExpireRemove();
	if (!hasInvited())
	{
		return false;
	}
	auto& inviteRef = inviteUserRef();
	auto teamId = _inviteTeamId;
	auto leaderSceneId = _inviteLeaderSceneId;
	resetInvite();
	if (pMem != nullptr)
	{
		return false;
	}
	if (inviteRef.pMem == nullptr)
	{
		return false;
	}
	auto& team = inviteRef.pMem->teamRef();
	team.teamLeaderRef().tryExpireRemove();
	if (team.getTeamId() != teamId)
	{
		return false;
	}
	if (inviteRef.pMem->isLeader())
	{
		if (team.isMemberFull())
		{
			return false;
		}
		team.createMember(this);
		//pMem->jumpToLeader();
	}
	else
	{
		//非队长邀请转为申请让队长审批
		if (team.teamLeaderRef().isApplyFull())
		{
			return false;
		}
		team.teamLeaderRef().createApply(this);
	}
	return true;
}

bool TeamUser::innerRefuseInvited()
{
	tryExpireRemove();
	if (!hasInvited())
	{
		return false;
	}
	resetInvite();
	return true;
}

bool TeamUser::innerApplyTeam(uint32 sceneHashId, zTeamIdType teamId, zSceneIdType leaderSceneId)
{
	tryExpireRemove();
	if (pMem != nullptr)
	{
		return false;
	}
	auto* pTeam = gTeamMgr->getTeam(teamId);
	if (pTeam == nullptr)
	{
		return false;
	}
	pTeam->teamLeaderRef().tryExpireRemove();
	if (pTeam->isMemberFull())
	{
		return false;
	}
	if (pTeam->teamLeaderRef().isApplyFull())
	{
		return false;
	}
	if (pTeam->isMatching() && getLevel() >= pTeam->getMinLevel() && getLevel() <= pTeam->getMaxLevel())
	{
		pTeam->createMember(this);
		//pMem->jumpToLeader();
	}
	else
	{
		pTeam->teamLeaderRef().createApply(this);
	}
	return true;
}

bool TeamUser::innerFollowLeader()
{
	return true;
}

bool TeamUser::innerCancelFollow()
{
	return true;
}

bool TeamUser::innerAgreeTogether(uint32 sceneHashId)
{
	tryExpireRemove();
	if (pMem == nullptr)
	{
		return false;
	}
	auto& team = pMem->teamRef();
	team.teamLeaderRef().tryExpireRemove();
	if (pMem->isLeader())
	{
		return false;
	}
	if (team.teamLeaderRef().getSceneId() != getSceneId())
	{
		return false;
	}
	if (!pMem->isFollow())
	{
		pMem->setFollow();
		pMem->broadChgFieldToScene({inner::TMFType_Follow});
		pMem->sendChgFieldToTeam({common::TMFieldType_Follow});
	}
	//pMem->jumpToLeader();
	return true;
}

bool TeamUser::innerRefuseTogether()
{
	if (pMem == nullptr)
	{
		return false;
	}
	pMem->teamRef().teamLeaderRef().tryExpireRemove();
	if (pMem->isLeader())
	{
		return false;
	}
	return true;
}

bool TeamUser::innerReplaceLeader()
{
	return true;
}

bool TeamUser::innerJumpToLeader(uint32 sceneHashId, uint32 eJumpTo)
{
	return true;
}