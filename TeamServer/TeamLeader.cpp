#include "team.pb.h"
#include "innerteam.pb.h"

#include "TeamUser.h"
#include "TeamMember.h"
#include "TeamLeader.h"
#include "TeamApply.h"
#include "SessionTeam.h"
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

void  TeamLeader::fill(common::TeamApplyList& out)
{
	for (auto* pApply : _applyList)
	{
		pApply->fill(*out.add_applys());
	}
}

void  TeamLeader::fill(common::RecruitTeamLeader& out)
{
	out.set_nickname(_pMem->pUser->getNickName());
	out.set_chatfont(_pMem->pUser->getChatFont());
	out.set_chatframe(_pMem->pUser->getChatFrame());
	_pMem->fill(*out.mutable_leader());
}

bool  TeamLeader::foreachApply(std::function<bool(TeamApply* pApply)> func)
{
	for (auto iter = _applyList.begin(); iter != _applyList.end();)
	{
		auto del = iter++;
		auto* pApply = *del;
		if (!func(pApply))
			return false;
	}
	return true;
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
	if (pUser->isTeamPersonMatching())
	{
		pUser->cancelMatchPerson();
	}
	auto* pTeam = memberRef().teamPtr();
	if (!pTeam->isMemberFull())
	{
		assert(pUser->pMem == nullptr);
		pTeam->createMember(pUser);
		deleteApply(pUser);
		//pUser->pMem->jumpToLeader();				//跳转到队长旁边
	}

}

void TeamLeader::refuseApply(TeamUser* pUser)
{
	assert(pUser != nullptr);
	deleteApply(pUser);
}

void TeamLeader::togetherMember(TeamMember* pMember)
{
	_pMem->setExpireTogether();
	_pMem->sendChgFieldToMe({common::TMFieldType_Together});
	client::ModuleTeam_Ntf_LeaderTogetherMember ntf;
	if (pMember != nullptr && _pMem->pUser->getEproc() == pMember->pUser->getEproc())
	{
		//pMember->sendCmdtToMe(client::enClientFirst_Team, client::enSecondTeam_Ntf_LeaderTogetherMember, ntf);
	}
	else
	{
		_pMem->teamRef().foreachMem([&](auto& memRef)->bool
			{
				if (memRef.isLeader()) return true;
				if (memRef.pUser->getEproc() != _pMem->pUser->getEproc()) return true;
				//memRef.sendCmeToMe(client::enClientFirst_Team, client::enSecondTeam_Ntf_LeaderTogetherMember, ntf);
				return true;
			});
	}
}

void TeamLeader::appointLeader(TeamMember* pMember)
{
	assert(pMember != _pMem);
	auto* pTeam = _pMem->teamPtr();
	assert(pMember->teamPtr() == pTeam);

	auto idx1 = _pMem->getIndex();
	auto idx2 = pMember->getIndex();

	_pMem->setIndex(idx2);
	pMember->setIndex(idx1);
	pMember->setExpireAppoint();
	auto* pOld = _pMem;
	_pMem = pMember;

	std::vector<uint32> v2{ common::TFieldType_LeaderId };
	if (pOld->pUser->getSceneId() != _pMem->pUser->getSceneId())
	{
		v2.push_back(common::TFieldType_LeaderSceneId);
	}
	pTeam->broadChgFieldToScene({ inner::TFType_LeaderId });
	pTeam->sendChgFieldToTeam(vector_to_initializer_list<uint32>(v2));

	pOld->broadChgFieldToScene({ inner::TMFType_Index });
	pOld->sendChgFieldToTeam({ common::TMFieldType_Index });

	std::vector<uint32> v3{ inner::TMFType_Index };
	std::vector<uint32> v4{ common::TMFieldType_Index };
	if (_pMem->isFollow())
	{
		_pMem->resetFollow();
		v3.push_back(inner::TMFType_Follow);
		v4.push_back(common::TMFieldType_Follow);
	}
	_pMem->broadChgFieldToScene(vector_to_initializer_list<uint32>(v3));
	_pMem->sendChgFieldToTeam(vector_to_initializer_list<uint32>(v4));
	//通知清空前队长申请列表
	client::ModuleTeam_Ntf_LeaderEmptyApply ntf1;
	//pOld->sendChgFieldToMe(client::enClientFirst_Team, client::enModuleTeam_Ntf_LeaderEmptyApply, ntf1);

	client::ModuleTeam_Ntf_LeaderAddApply ntf2;
	fill(*ntf2.mutable_applylist());
	//_pMem->sendChgFieldToMe(client::enClientFirst_Team, client::enModuleTeam_Ntf_LeaderAddApply, ntf2);
	_pMem->sendChgFieldToMe({ common::TMFieldType_Appoint });
}

void TeamLeader::replaceLeader(TeamMember* pMember)
{
	assert(pMember != _pMem);
	auto* pTeam = _pMem->teamPtr();
	assert(pMember->teamPtr() == pTeam);

	auto idx1 = _pMem->getIndex();
	auto idx2 = pMember->getIndex();

	_pMem->setIndex(idx2);
	pMember->setIndex(idx1);
	pMember->setExpireReplace();
	auto* pOld = _pMem;
	_pMem = pMember;

	std::vector<uint32> v2{common::TFieldType_LeaderId};
	if (pOld->pUser->getSceneId() != _pMem->pUser->getSceneId())
	{
		v2.push_back(common::TFieldType_LeaderSceneId);
	}
	pTeam->broadChgFieldToScene({inner::TFType_LeaderId});
	pTeam->sendChgFieldToTeam(vector_to_initializer_list<uint32>(v2));

	pOld->broadChgFieldToScene({inner::TMFType_Index});
	pOld->sendChgFieldToTeam({common::TMFieldType_Index});

	std::vector<uint32> v3{inner::TMFType_Index};
	std::vector<uint32> v4{common::TMFieldType_Index};
	if (_pMem->isFollow())
	{
		_pMem->resetFollow();
		v3.push_back(inner::TMFType_Follow);
		v4.push_back(common::TMFieldType_Follow);
	}
	_pMem->broadChgFieldToScene(vector_to_initializer_list<uint32>(v3));
	_pMem->sendChgFieldToTeam(vector_to_initializer_list<uint32>(v4));
	//通知清空前队长申请列表
	client::ModuleTeam_Ntf_LeaderEmptyApply ntf1;
	//pOld->sendChgFieldToMe(client::enClientFirst_Team, client::enModuleTeam_Ntf_LeaderEmptyApply, ntf1);

	client::ModuleTeam_Ntf_LeaderAddApply ntf2;
	fill(*ntf2.mutable_applylist());
	//_pMem->sendChgFieldToMe(client::enClientFirst_Team, client::enModuleTeam_Ntf_LeaderAddApply, ntf2);
	_pMem->sendChgFieldToMe({common::TMFieldType_Replace});
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
