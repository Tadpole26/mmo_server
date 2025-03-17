#include "team.pb.h"

#include "TeamUser.h"
#include "TeamMember.h"
#include "TeamLeader.h"
#include "TeamApply.h"
#include "ParamConfig.h"

TeamLeader::TeamLeader(TeamMember* pMember) 
	: _pMem(pMember)
{
}

TeamLeader::~TeamLeader()
{
}

void TeamLeader::final()
{
	__deleteApplyAll();
}

bool TeamLeader::isApplyFull()
{
	return _applyList.size() >= gParamCfg->teamApplyCountMax;
}

bool TeamLeader::hasApply(TeamUser* pUser)
{
	return _applyMap.find(pUser) != _applyMap.end();
}

void TeamLeader::createApply(TeamUser* pUser)
{
	if (!hasApply(pUser))
	{
		auto* apply = new TeamApply(pUser, getSceneId());
		assert(apply != nullptr);
		__addApply(apply);
		//通知玩家新申请增加
		client::ModuleTeam_Ntf_LeaderAddApply ntf;
		apply->fill(*(ntf.mutable_applylist()->add_applys()));
		//sendCmdToMe(client::enClientFirt_Team, client::enSecondTeam_Ntf_LeaderAddApply, ntf);
	}
}

void TeamLeader::deleteApply(TeamUser* pUser)
{
	__deleteApply(pUser);
	client::ModuleTeam_Ntf_LeaderRemoveApply ntf;
	ntf.add_roleids(pUser->getRoleId());
	//sendCmdToMe(client::enClientFirt_Team, client::ecModuleTeam_Ntf_LeaderRemoveApply, ntf);
}

void TeamLeader::deleteApplyAll()
{
	__deleteApplyAll();
	client::ModuleTeam_Ntf_LeaderEmptyApply ntf;
	//sendCmdToMe(client::enClientFirt_Team, client::enModuleTeam_Ntf_LeaderEmptyApply, ntf);
}

void TeamLeader::tryExpireRemove()
{
	for (auto iter = _applyList.begin(); iter != _applyList.end();)
	{
		auto del = iter++;
		auto* apply = *del;
		if (!apply->isExpire())
			break;
		__deleteApply(apply->pUser);
	}
}

void TeamLeader::agreeApply(TeamUser* pUser)
{
	assert(pUser != nullptr);

}

void TeamLeader::refuseApply(TeamUser* pUser)
{
}

void TeamLeader::togetherMember(TeamMember* pMember)
{
}

void TeamLeader::appointLeader(TeamMember* pMember)
{
}

void TeamLeader::replaceLeader(TeamMember* pMember)
{
}

void TeamLeader::__addApply(TeamApply* pApply)
{
	assert(pApply != nullptr);
	_applyList.push_back(pApply);
	_applyMap[pApply->pUser] = (--_applyList.end());
}

void TeamLeader::__deleteApply(TeamUser* pUser)
{
	assert(pUser != nullptr);
	auto iter = _applyMap.find(pUser);
	if (iter != _applyMap.end())
	{
		auto* apply = *(iter->second);
		_applyList.erase(iter->second);
		_applyMap.erase(pUser);
		apply->final();
		SAFE_DELETE(apply);
	}
}

void TeamLeader::__deleteApplyAll()
{
	for (auto iter = _applyList.begin(); iter != _applyList.end();)
	{
		auto del = iter++;
		auto* apply = *del;
		__deleteApply(apply->pUser);
	}
}
