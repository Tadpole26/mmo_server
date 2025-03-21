#include "innercommon.pb.h"
#include "teamcommon.pb.h"
#include "team.pb.h"
#include "innerteam.pb.h"

#include "TeamUser.h"
#include "TeamUserMgr.h"
#include "TeamMgr.h"
#include "SessionTeam.h"
#include "TeamVote.h"
#include "TeamConfig.h"

bool TeamMgr::validMatchSize(uint32 sz)
{
	return sz > 0 && sz < gTeamCfg->maxTeamPlayer;
}

uint32 TeamMgr::sizeIndex(uint32 sz)
{
	assert(validMatchSize(sz));
	return sz - 1;
}

TeamMgr::TeamMgr()
{
}

TeamMgr::~TeamMgr()
{
}

void TeamMgr::final()
{
}

bool TeamMgr::init()
{
	if (gTeamCfg->maxTeamPlayer == 0)
	{
		Log_Error("最大队伍人数为0");
		return false;
	}
	Log_Info("初始化队伍最大人数:%u", gTeamCfg->maxTeamPlayer);
	if (!gTeamCfg->foreach([&](config::team_info_t& cfg)->bool
		{
			if (cfg.lvMin == 0)
			{
				Log_Error("最小等级为0,%u", cfg.tid);
				return false;
			}
			if (cfg.lvMax == 0)
			{
				Log_Error("最大等级为0,%u", cfg.tid);
				return false;
			}
			if (cfg.lvMax < cfg.lvMin)
			{
				Log_Error("最大等级小于最小等级,%u", cfg.tid);
				return false;
			}
			return createTargetQueue(cfg);
		}))
	{
		Log_Error("初始化队伍目标失败");
		return false;
	}
	Log_Info("初始化队伍目标成功,%u", sizeTgQueue());
	return true;
}

void TeamMgr::tick()
{
	//队伍匹配个人填充队伍
	if (!_activeLvQueue.empty())
	{
		for (auto iter = _activeLvQueue.begin(); iter != _activeLvQueue.end();)
		{
			auto del = iter++;
			auto* pLvQueue = *del;
			if (pLvQueue->tick())
				break;
		}
	}
	//投票过期
	if (!_allVoteQueue.empty())
	{
		for (auto iter = _allVoteQueue.begin(); iter != _allVoteQueue.end();)
		{
			auto del = iter++;
			auto* pVote = *del;
			if (!pVote->tick()) continue;
			_allVoteQueue.erase(del);
			_allVote.erase(pVote->getUid());
			pVote->final();
			SAFE_DELETE(pVote);
		}
	}
}

SessionTeam* TeamMgr::createTeam()
{
	auto* team = new SessionTeam(__createTeamId());
	assert(team != nullptr);
	_allTeam[team->getTeamId()] = team;
	Log_Info("createTeam,%lu", team->getTeamId());
	return team;
}

SessionTeam* TeamMgr::getTeam(zTeamIdType teamId)
{
	auto iter = _allTeam.find(teamId);
	if (iter == _allTeam.end())
		return nullptr;
	return iter->second;
}

void TeamMgr::destroyTeam(zTeamIdType teamId)
{
	auto iter = _allTeam.find(teamId);
	if (iter != _allTeam.end())
	{
		auto* pTeam = iter->second;
		_allTeam.erase(iter);
		Log_Info("destroyTeam,%lu", pTeam->getTeamId());
		pTeam->final();
		SAFE_DELETE(pTeam);
		//broadRemoveTeamScene(teamId);
	}
}

//void batCreateTeam(uint32 sceneHashId, const inner::InnerCopyTeamList* in);
void TeamMgr::batDestroyTeam(std::initializer_list<zTeamIdType> teamList)
{
}

void TeamMgr::batLeaveTeam(const inner::InnerRoleIdList& roleList)
{
	std::vector<TeamUser*> vecRoleList;
	for (auto i = 0; i < roleList.roleids_size(); ++i)
	{
		auto roleId = roleList.roleids(i);
		auto* pUser = gTeamUserMgr->getRole(roleId);
		assert(pUser != nullptr);
		if (pUser->pMem)
		{
			vecRoleList.push_back(pUser);
		}
	}
	if (!vecRoleList.empty())
	{
		batLeaveTeam(vector_to_initializer_list<TeamUser*>(vecRoleList), common::enLeaveTeamType_Active);
	}
}

void TeamMgr::batLeaveTeam(std::initializer_list<TeamUser*> roleList, uint32 eLeave)
{
	for (auto* pUser : roleList)
	{
		if (pUser->pMem != nullptr)
			pUser->leaveTeam(eLeave);
			
	}
}

bool TeamMgr::createTargetQueue(const config::team_info_t& cfg)
{
	return _allTgQueue.insert(std::make_pair(cfg.tid, new TeamTargetQueue(cfg))).second;
}

TeamTargetQueue& TeamMgr::tgQueueRef(const config::team_info_t& cfg)
{
	return tgQueueRef(cfg.tid);
}

TeamTargetQueue& TeamMgr::tgQueueRef(uint32 targetId)
{
	auto iter = _allTgQueue.find(targetId);
	assert(iter != _allTgQueue.end());
	return *iter->second;
}

void TeamMgr::addLvQueueToActive(TeamLevelQueue* pLvQueue)
{
	assert(pLvQueue != nullptr);
	if (pLvQueue->isActive())
	{
		auto& p = pLvQueue->activeLvQueuePair;
		if (!p.first)
		{
			_activeLvQueue.push_back(pLvQueue);
			p.first = true;
			p.second = (--_activeLvQueue.end());
		}
	}
}

void TeamMgr::removeLvQueueFromActive(TeamLevelQueue* pLvQueue)
{
	assert(pLvQueue != nullptr);
	if (!pLvQueue->isActive())
	{
		auto& p = pLvQueue->activeLvQueuePair;
		if (p.first)
		{
			_activeLvQueue.erase(p.second);
			p.first = false;
			p.second = _activeLvQueue.end();
		}
	}
}

TeamVote* TeamMgr::createVote()
{
	return createVote(0, true);
}

TeamVote* TeamMgr::createVote(uint32 lastingTime, bool agreeByDefault)
{
	auto uid = __createVoteId();
	if (_allVote.find(uid) != _allVote.end())
	{
		return nullptr;
	}
	auto* pVote = new TeamVote(uid);
	if (!addTeamVote(pVote))
	{
		pVote->final();
		SAFE_DELETE(pVote);
		return nullptr;
	}
	pVote->setLastingTime(lastingTime);
	pVote->setAgreeByDefault(agreeByDefault);
	return pVote;
}

TeamVote* TeamMgr::getVote(zVoteIdType voteUid)
{
	auto iter = _allVote.find(voteUid);
	if (iter == _allVote.end()) return nullptr;
	return iter->second;
}

bool TeamMgr::addTeamVote(TeamVote* pVote)
{
	assert(pVote != nullptr);
	assert(pVote->listIt == getVoteItEnd());;
	if (!_allVote.emplace(pVote->getUid(), pVote).second)
		return false;
	_allVoteQueue.push_back(pVote);
	pVote->setListIt(--_allVoteQueue.end());
	return true;
}

void TeamMgr::removeTeamVote(TeamVote* pVote)
{
	assert(pVote != nullptr);
	assert(pVote->listIt != getVoteItEnd());
	_allVote.erase(pVote->getUid());
	_allVoteQueue.erase(pVote->listIt);
	pVote->setListIt(getVoteItEnd());
}

zTeamIdType TeamMgr::__createTeamId()
{
	if (_idpair.first != GetCurrTime())
	{
		_idpair.first = GetCurrTime();
		_idpair.second = 1;
	}
	return cTeam::unionTeamId(520, _idpair.first, _idpair.second++);
}

zVoteIdType TeamMgr::__createVoteId()
{
	while (!++_voteIncreaseId);
	return _voteIncreaseId;
}