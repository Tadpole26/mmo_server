#include "team.pb.h"
#include "innerteam.pb.h"

#include "TeamUser.h"
#include "TeamMember.h"
#include "TeamLeader.h"
#include "SessionTeam.h"
#include "TeamMgr.h"
#include "TeamConfig.h"
#include "ParamConfig.h"

SessionTeam::SessionTeam(zTeamIdType teamId) : _teamId(teamId)
{
	for (uint32 idx = 1; idx <= gTeamCfg->maxTeamPlayer; ++idx)
	{
		_indexs.push_back(idx);
	}
}

SessionTeam::~SessionTeam()
{
}

void SessionTeam::final()
{
	if (_leader != nullptr)
	{
		_leader->final();
		SAFE_DELETE(_leader);
	}
}

void SessionTeam::fill(common::PlatTargetTeam& out)
{
	out.set_teamid(_teamId);
	out.set_leadername(_leader->memberRef().pUser->getNickName());
	out.set_minlevel(getMinLevel());
	out.set_maxlevel(getMaxLevel());
	foreachMem([&](auto& memRef)->bool
	{
		memRef.fill(*out.add_members());
		return true;
	});
	out.set_leadersceneid(_leader->getSceneId());
}

void SessionTeam::fill(common::TeamBase& out)
{
	out.set_teamid(_teamId);
	out.set_leaderid(_leader->memberPtr()->pUser->getRoleId());
	out.set_targetid(getTargetId());
	out.set_minlevel(getMinLevel());
	out.set_maxlevel(getMaxLevel());
	out.set_matchtime(getMatchTime());
	out.set_text(getText());
	out.set_leadersceneid(teamLeaderRef().getSceneId());
}

void SessionTeam::fill(common::TeamMemberList& out)
{
	foreachMem([&](auto& memRef)->bool
	{
		memRef.fill(*out.add_members());
		return true;
	});
}

void SessionTeam::fill(common::TeamFieldList& out, uint32 fieldId)
{
	switch (fieldId)
	{
	case common::TFieldType_LeaderId: { auto& one = *out.add_tfid(); one.set_type(fieldId); one.set_val(getLeaderId()); } break;
	case common::TFieldType_TargetId: { auto& one = *out.add_tfid(); one.set_type(fieldId); one.set_val(getTargetId()); } break;
	case common::TFieldType_MinLevel: { auto& one = *out.add_tfid(); one.set_type(fieldId); one.set_val(getMinLevel()); } break;
	case common::TFieldType_MaxLevel: { auto& one = *out.add_tfid(); one.set_type(fieldId); one.set_val(getMaxLevel()); } break;
	case common::TFieldType_MatchTime: { auto& one = *out.add_tfid(); one.set_type(fieldId); one.set_val(getMatchTime()); } break;
	case common::TFieldType_Text: { auto& one = *out.add_tfbyte(); one.set_type(fieldId); one.set_val(getText()); } break;
	case common::TFieldType_LeaderSceneId: { auto& one = *out.add_tfid(); one.set_type(fieldId); one.set_val(_leader->getSceneId()); } break;
	defaut: assert(false);		break;
	}
}

void SessionTeam::fill(common::RecruitTeam& out)
{
	out.set_teamid(getTeamId());
	out.set_targetid(getTargetId());
	out.set_minlevel(getMinLevel());
	out.set_maxlevel(getMaxLevel());
	out.set_text(getText());
	out.set_leadersceneid(teamLeaderRef().getSceneId());
	_leader->fill(*out.mutable_leader());
	foreachMem([&](auto& memRef)->bool
	{
		if (!memRef.isLeader())
			memRef.fill(*out.add_members());
		return true;
	});
}

bool SessionTeam::broadChgFieldToScene(std::initializer_list<uint32> changes)
{
	//广播到scenesvr
	return true;
}

bool SessionTeam::broadAddMemeberToScene(TeamMember* pMember)
{
	//广播到scenesvr
	return true;
}

bool SessionTeam::broadRemoveMemberToScene(zRoleIdType roleId, uint32 type)
{
	//广播到scenesvr
	return true;
}

bool SessionTeam::broadRecruitToWorld()
{
	setRecruitExpire();
	client::ModuleTeam_Ntf_TeamRecruitInfo ntf;
	fill(*ntf.mutable_recruit());
	return true;
}

bool SessionTeam::sendCmdToTeam(const uint32 first, const uint32 second, const google::protobuf::Message& in)
{
	return true;
}

bool SessionTeam::sendCmdToTeamExcept(TeamMember* pExcept, const uint32 first, const uint32 second, const google::protobuf::Message& in)
{
	return foreachMem([&](auto& memRef)->bool
		{
			if (&memRef == pExcept) return true;
			//memeRef.sendCmdToMe(first, second, in);
			return true;
		});
}

bool SessionTeam::sendChgFieldToTeam(std::initializer_list<uint32> changes)
{
	client::ModuleTeam_Ntf_UpdateTeam ntf;
	for (auto fieldId : changes)
	{
		fill(*ntf.mutable_fieldlist(), fieldId);
	}
	//return sendCmdToTeam(client::enClientFirt_Team, client::enSecondTeam_Ntf_UpdateTeam, ntf);
	return true;
}

void SessionTeam::fill(inner::InnerTeam& out)
{
	out.set_teamid(getTeamId());
	out.set_leadid(getLeaderId());
	fill(*out.mutable_base());
	auto& mmap = *out.mutable_mlist();
	foreachMem([&](auto& memRef)->bool
	{
		memRef.fill(mmap[memRef.pUser->getRoleId()]);
		return true;
	});
}

void SessionTeam::fill(inner::InnerTeamBase& out)
{
	out.set_targetid(getTargetId());
	out.set_minlevel(getMinLevel());
	out.set_maxlevel(getMaxLevel());
	out.set_matchtime(getMatchTime());
	//out.set_midwayjoin(checkMidwwayJoin());
}

void SessionTeam::fill(inner::InnerTeamField& out, uint32 fieldId)
{
	switch (fieldId)
	{
	case inner::TFType_LeaderId: out.set_leaderid(getLeaderId()); break;
	case inner::TFType_TargetId: out.set_targetid(getTargetId());  break;
	case inner::TFType_MinLevel: out.set_minlevel(getMinLevel());  break;
	case inner::TFType_MaxLevel: out.set_maxlevel(getMaxLevel()); break;
	case inner::TFType_MatchTime: out.set_matchtime(getMatchTime()); break;
	case inner::TFType_MidwayJoin: out.set_midwayjoin(checkMidwayJoin()); break;
	defaut: assert(false);		break;
	}
}

bool SessionTeam::isMemberEmpty()
{
	return sizeMember() == 0;
}

bool SessionTeam::isMemberFull()
{
	return sizeMember() >= gTeamCfg->maxTeamPlayer;
}

bool SessionTeam::isLeader(TeamMember* pMember)
{
	return pMember != nullptr && _leader->memberPtr() == pMember;
}

bool SessionTeam::isLeader(TeamUser* pUser)
{
	return pUser != nullptr && _leader->memberPtr() == pUser->pMem;
}

bool SessionTeam::isLeader(zRoleIdType roleId)
{
	return getLeaderId() == roleId;
}

bool SessionTeam::isMember(TeamMember* pMember)
{
	return pMember != nullptr && pMember->teamPtr() == this;
}

bool SessionTeam::isMember(TeamUser* pUser)
{
	return pUser != nullptr && pUser->pMem != nullptr && pUser->pMem->teamPtr() == this;
}

bool SessionTeam::isMember(zRoleIdType roleId)
{
	auto* pMember = getMember(roleId);
	return pMember != nullptr && pMember->teamPtr() == this;
}

bool SessionTeam::isMatching()
{
	return false;
}

bool SessionTeam::isVoting()
{
	return false;
}

bool SessionTeam::isRecruitExpire()
{
	return _recruitExpire <= GetCurrTime();
}

bool SessionTeam::checkMidwayJoin(zSceneIdType leaderSceneId)
{
	return true;
}

zRoleIdType SessionTeam::getLeaderId()
{
	return (_leader && _leader->memberPtr()) ? _leader->memberRef().pUser->getRoleId() : 0;
}

TeamMember* SessionTeam::getMember(zRoleIdType roleId)
{
	TeamMember* pFind = nullptr;
	foreachMem([&](auto& memRef)->bool 
	{
		if (memRef.pUser->getRoleId() == roleId)
		{
			pFind = &memRef;
			return false;
		}
		return true;
	});
	return pFind;
}

TeamMember* SessionTeam::leaderPtr()
{
	return _leader->memberPtr();
}

void SessionTeam::createLeader(TeamUser* pUser)
{
	assert(_leader == nullptr);
	assert(pUser != nullptr);
	//if (pUser->isTeamPersonMatching())
	//	pUser->cancelMatchPerson();
	pUser->setExpireCreate();								//创建队伍队长
	auto* pMember = __createMember(pUser);
	pMember->setFollow();
	_leader = new TeamLeader(pMember);

	addToSzQueueTarget();
	//teamMgr::get().broadAddTeamToScene(this);
	client::ModuleTeam_Ntf_CreateTeam ntf;
	fill(*ntf.mutable_base());
	fill(*ntf.mutable_memberlist());
	pUser->fill(*ntf.mutable_notteam());
	pUser->fill(*ntf.mutable_ownteam());
	//pMember->sendCmdToMe(client::enClientFirst_Team, client::enSecondTeam_Ntf_CreateTeam, ntf);
}

void SessionTeam::createMember(TeamUser* pUser)
{
	assert(!isMemberFull());
	assert(pUser != nullptr);
	//if (pUser->isTeamPersonMatching())
	//	pUser->cancelMatchPerson();
	auto* pMember = __createMember(pUser);
	broadAddMemeberToScene(pMember);

	auto bmatch = isMatching();
	if (isMemberFull() && bmatch)			
	{
		_leader->deleteApplyAll();
		cancelMatch();
		removeFromSzQueueTarget();//人数够了,队伍从显示面板删除
	}
	else if(isMemberFull() && !bmatch)
	{
		_leader->deleteApplyAll();
		removeFromSzQueueTarget();//人数够了,队伍从显示面板删除
	}
	else if (bmatch)
	{
		removeFromSzQueueLevelAll();
		removeFromSzQueueTarget();

		addToSzQueueTarget();	//人数不够,队伍人数变化,更新显示面板
		addToSzQueueLevelAll();	//人数不够,队伍人数变化,更新匹配队列
	}
	else
	{
		removeFromSzQueueTarget();
		addToSzQueueLevelAll();	//人数不够,队伍人数变化,更新匹配队列
	}

	client::ModuleTeam_Ntf_AddMember ntf1;
	pMember->fill(*(ntf1.mutable_memeberlist()->add_members()));
	//sendCmdToTeamExcept(pMember, client::enClientFirst_Team);

	client::ModuleTeam_Ntf_SelfJoinTeam ntf2;
	fill(*ntf2.mutable_base());
	fill(*ntf2.mutable_memberlist());
	pUser->fill(*ntf2.mutable_notteam());
	pUser->fill(*ntf2.mutable_ownteam());
	//pMember->sendCmdToMe(client::enClientFirst_Team, client::enSecondTeam_Ntf_SelfJoinTeam, ntf2);
}

void SessionTeam::setTarget(uint32 targetId)
{
	_targetId = targetId;
}

void SessionTeam::setMinLevel(uint32 minLevel)
{
	_minLevel = minLevel;
}

void SessionTeam::setMaxLevel(uint32 maxLevel)
{
	_maxLevel = maxLevel;
}

void SessionTeam::setText(const std::string& text)
{
	_text = text;
}

void SessionTeam::setMatchTime()
{
	_matchTime = GetCurrTime();
}

void SessionTeam::resetMatchTime()
{
	_matchTime = 0;
}

void SessionTeam::setRecruitExpire()
{
	_recruitExpire = GetCurrTime() + gParamCfg->teamAutoRecruitCold;
}

void SessionTeam::resetRecruitExpire()
{
	_recruitExpire = 0;
}

void SessionTeam::setMidwayJoin(uint32 join)
{
}

bool SessionTeam::foreachMem(std::function<bool(TeamMember& memRef)> func)
{
	if (!leaderPtr()) return false;
	if (!func(*leaderPtr())) return false;
	for (auto iter = _members.begin(); iter != _members.end();)
	{
		auto del = iter++;
		auto* pMem = *del;
		if (!pMem->isLeader() && !func(*pMem))
			return false;
	}
	return true;
}

bool SessionTeam::foreachMemLess(std::function<bool(TeamMember& memRef)> func)
{
	if (!leaderPtr()) return false;
	for (auto iter = _members.begin(); iter != _members.end();)
	{
		auto del = iter++;
		auto* pMem = *del;
		if (!func(*pMem))
			return false;
	}
	return func(*leaderPtr());
}

void SessionTeam::starMatch()
{
	assert(!isMatching());
	addToSzQueueLevelAll();				//加入匹配队列
	setMatchTime();
	broadChgFieldToScene({inner::TFType_MatchTime});
	sendChgFieldToTeam({common::TFieldType_MatchTime});
	//broadRecruitToWorld();

}

void SessionTeam::cancelMatch()
{
	removeFromSzQueueLevelAll();		//从匹配队列中删除
	resetMatchTime();
	broadChgFieldToScene({ inner::TFType_MatchTime });
	sendChgFieldToTeam({ common::TFieldType_MatchTime });
	//resetRecruitExpire();
	//gTeamMgr->removeAutoRecruitTeam(this);
}

//最后一人离开队伍
void SessionTeam::leaveMemberLast()
{
	assert(sizeMember() == 1);
	auto* pMember = _leader->memberPtr();
	auto* pUser = pMember->pUser;
	if (isMatching())
	{
		cancelMatch();
	}
	_leader->resetMember();
	__removeMember(pMember);
	pMember->final();
	SAFE_DELETE(pMember);

	removeFromSzQueueTarget();
	//broadRemoveMemberToScene(pUser->getRoleId(), common::enLeaveTeamType_Active);
	client::ModuleTeam_Ntf_LeaveTeam ntf;
	ntf.set_type(common::enLeaveTeamType_Active);
	//pUser->sendCmdToMe();
	//evTeamMemberLeave(pUser);		//如果pvp匹配,强制退出匹配
}

void SessionTeam::leaveMember(TeamMember* pMember)
{
	assert(pMember != nullptr);
	assert(!pMember->isLeader());
	auto* pUser = pMember->pUser;
	auto idx = pMember->getIndex();
	auto bmatch = isMatching();
	auto bfull = isMemberFull();
	__removeMember(pMember);
	pMember->final();
	SAFE_DELETE(pMember);
	if (bmatch)
	{
		removeFromSzQueueLevelAll();
	}
	if (!bfull)
	{
		removeFromSzQueueTarget();
	}
	addToSzQueueTarget();
	if (bmatch)
	{
		addToSzQueueLevelAll();
	}
	broadRemoveMemberToScene(pUser->getRoleId(), common::enLeaveTeamType_Active);

	client::ModuleTeam_Ntf_RemoveMember ntf2;
	ntf2.add_roleids(pUser->getRoleId());
	//sendCmdToTeam(client::enClientFirst_Team, client::enSecondTeam_Ntf_RemoveMember, ntf2);

	client::ModuleTeam_Ntf_LeaveTeam ntf1;
	//ntf1.set_type(common::LeaveTeamType_Active);
	//pUser->sendCmdToMe(Client::enClientFirst_Team, client::enSecondTeam_Ntf_LeaveTeam, ntf1);

	changeMemberIndex(idx);
}

void SessionTeam::fireMember(TeamMember* pMember)
{
	assert(pMember != nullptr);
	assert(!pMember->isLeader());
	auto* pUser = pMember->pUser;
	auto idx = pMember->getIndex();
	auto bmatch = isMatching();
	auto bfull = isMemberFull();
	__removeMember(pMember);
	pMember->final();
	SAFE_DELETE(pMember);
	if (bmatch)
	{
		removeFromSzQueueLevelAll();
	}
	if (!bfull)
	{
		removeFromSzQueueTarget();
	}
	addToSzQueueTarget();
	if (bmatch)
	{
		addToSzQueueLevelAll();
	}
	broadRemoveMemberToScene(pUser->getRoleId(), common::enLeaveTeamType_Active);
	client::ModuleTeam_Ntf_RemoveMember ntf2;
	ntf2.add_roleids(pUser->getRoleId());
	//sendCmdToTeam(client::enClientFirst_Team, client::enSecondTeam_Ntf_RemoveMember, ntf2);

	client::ModuleTeam_Ntf_LeaveTeam ntf1;
	//ntf1.set_type(common::LeaveTeamType_Active);
	//pUser->sendCmdToMe(Client::enClientFirst_Team, client::enSecondTeam_Ntf_LeaveTeam, ntf1);

	changeMemberIndex(idx);
}

void SessionTeam::changeMemberIndex(uint32 index)
{
	for (auto iter = _members.begin(); iter != _members.end();)
	{
		auto del = iter++;
		auto* pMember = *del;
		if (pMember->getIndex() > index)
		{
			_indexs.push_back(pMember->getIndex());
			_indexs.sort();
			pMember->setIndex(_indexs.front());
			_indexs.pop_front();
			//pMember->sendChgFieldToTeam({common::TMFieldType_Index});
			//pMember->broadChgFieldToScene({inner::TMFType_Index});
		}
	}
}

void SessionTeam::cancelVote()
{
}

void SessionTeam::addToSzQueueTarget()
{
	//人满队伍不进入展示列表(展示给玩家加入使用)
	assert(!isMemberFull());
	auto* pSzQueueTg = szQueueTargetPair.first;
	assert(pSzQueueTg == nullptr);
	pSzQueueTg = gTeamMgr->tgQueueRef(getTargetId()).szQueuePtr(sizeMember());
	pSzQueueTg->addTeam(this);
	auto* pTgQueue = pSzQueueTg->pTgQueue;
	Log_Info("add to sz queue target suceess.targetId:%u,sz:%u,size:%u", pTgQueue->getTargetId(), pSzQueueTg->getKey(), pSzQueueTg->sizeTeam());
}

void SessionTeam::removeFromSzQueueTarget()
{
	auto* pSzQueueTg = szQueueTargetPair.first;
	pSzQueueTg->removeTeam(this);
	auto* pTgQueue = pSzQueueTg->pTgQueue;
	Log_Info("remove to sz queue target suceess.targetId:%u,sz:%u,size:%u", pTgQueue->getTargetId(), pSzQueueTg->getKey(), pSzQueueTg->sizeTeam());
}

void SessionTeam::addToSzQueueLevelAll()
{
	assert(!isMemberEmpty());
	assert(!isMemberFull());
	auto* pSzQueueTg = szQueueTargetPair.first;
	assert(pSzQueueTg != nullptr);
	auto* pTgQueue = pSzQueueTg->pTgQueue;
	assert(pTgQueue != nullptr);
	for (auto lv = getMinLevel(); lv <= getMaxLevel(); ++lv)
	{
		auto* pSzQueueLv = pTgQueue->lvQueueRef(lv).szQueuePtr(pSzQueueTg->getKey());
		pSzQueueLv->addTeam(this);
	}
	Log_Info("add to sz queue level suceess.targetId:%u,sz:%u,size:%u", pTgQueue->getTargetId(), pSzQueueTg->getKey(), pSzQueueTg->sizeTeam());
}

void SessionTeam::removeFromSzQueueLevelAll()
{
	for (auto iter = szQueueLevelList.begin(); iter != szQueueLevelList.end();)
	{
		auto del = iter++;
		auto& p = *del;
		auto* pSzQueueLv = p.first;
		p.first->removeTeam(this);
	}
	auto* pSzQueueTg = szQueueTargetPair.first;
	auto* pTgQueue = pSzQueueTg->pTgQueue;
	Log_Info("remove to sz queue level suceess.targetId:%u,sz:%u,size:%u", pTgQueue->getTargetId(), pSzQueueTg->getKey(), pSzQueueTg->sizeTeam());
}

TeamMember* SessionTeam::__createMember(TeamUser* pUser)
{
	auto index = _indexs.front();
	_indexs.pop_front();
	auto* pMember = new TeamMember;
	__addMember(pMember, index);
	pMember->pUser = pUser;
	pUser->pMem = pMember;
	pUser->resetInvite();					//成为成员,删除邀请信息
	pUser->resetExpireApply();				//成为成员,申请过期清除
	pUser->resetExpireCreate();				//成为成员,创建过期清除
	//pUser->resetExpireTarget();
	Log_Debug("create team member sucess.%u", index);
	return pMember;
}

void SessionTeam::__addMember(TeamMember* pMember, uint32 index)
{
	assert(pMember != nullptr);
	assert(pMember->teamPtr() == nullptr);
	_members.push_back(pMember);
	pMember->__setTeam(this);
	pMember->setIndex(index);
}

void SessionTeam::__removeMember(TeamMember* pMember)
{
	assert(pMember != nullptr);
	assert(pMember->teamPtr() == this);
	assert(pMember->pUser->pMem != nullptr);
	auto iterFind = std::find(_members.begin(), _members.end(), pMember);
	assert(iterFind != _members.end());
	_members.erase(iterFind);
	_indexs.push_back(pMember->getIndex());
	_indexs.sort();
	pMember->__setNotTeam();
	pMember->pUser->pMem = nullptr;
	Log_Debug("remove team member sucess.");
}