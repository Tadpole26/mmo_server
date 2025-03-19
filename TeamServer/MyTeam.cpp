#include "TeamUser.h"
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