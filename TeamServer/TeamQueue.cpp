#include "teamcommon.pb.h"

#include "TeamUser.h"
#include "TeamMember.h"
#include "TeamLeader.h"
#include "SessionTeam.h"
#include "TeamMgr.h"
#include "TeamConfig.h"
#include "ParamConfig.h"


TeamSizeQueueTarget::TeamSizeQueueTarget(uint32 key, TeamTargetQueue* pTgQueue) :
	_key(key),
	pTgQueue(pTgQueue)
{
}

TeamSizeQueueTarget::~TeamSizeQueueTarget()
{
}

void TeamSizeQueueTarget::final()
{
}

void TeamSizeQueueTarget::addTeam(SessionTeam* team)
{
	assert(team != nullptr);
	auto& p = team->szQueueTargetPair;
	assert(p.first == nullptr);
	_allTeam.push_back(team);
	p.first = this;
	p.second = (--_allTeam.end());
}

void TeamSizeQueueTarget::removeTeam(SessionTeam* team)
{
	assert(team != nullptr);
	auto& p = team->szQueueTargetPair;
	assert(p.first == this);
	_allTeam.erase(p.second);
	p.first = nullptr;
	p.second = _allTeam.end();
}

void TeamSizeQueueTarget::foreachTeam(std::function<bool(SessionTeam* team)> func)
{
	for (auto* team : _allTeam)
	{
		if (!func(team))
			break;
	}
}

TeamSizeQueueLevel::TeamSizeQueueLevel(uint32 key, TeamLevelQueue* pLvQueue) :
	_key(key),
	pLvQueue(pLvQueue)
{
}

TeamSizeQueueLevel::~TeamSizeQueueLevel()
{
}

void TeamSizeQueueLevel::final()
{

}

void TeamSizeQueueLevel::addTeam(SessionTeam* team)
{
	assert(team != nullptr);
	_allTeam.push_back(team);
	team->szQueueLevelList.emplace_back(this, --_allTeam.end());
	//加入激活队列
	gTeamMgr->addLvQueueToActive(this->pLvQueue);
}

void TeamSizeQueueLevel::removeTeam(SessionTeam* team)
{
	assert(team != nullptr);
	for (auto iter = team->szQueueLevelList.begin(); iter != team->szQueueLevelList.end();)
	{
		auto del = iter++;
		auto& p = *del;
		if (p.first == this)
		{
			_allTeam.erase(p.second);
			team->szQueueLevelList.erase(del);
			break;
		}
	}
}

SessionTeam* TeamSizeQueueLevel::front()
{
	return _allTeam.front();
}

TeamLevelQueue::TeamLevelQueue(uint32 lv, TeamTargetQueue* pTgQueue) :
	_key(lv),
	pTgQueue(pTgQueue)
{
	_allSzQueue.resize(gTeamCfg->maxTeamPlayer - 1, nullptr);
	for (auto sz = 1; sz < gTeamCfg->maxTeamPlayer; ++sz)
	{
		_allSzQueue[gTeamMgr->sizeIndex(sz)] = new TeamSizeQueueLevel(sz, this);
	}
}

TeamLevelQueue::~TeamLevelQueue()
{
}

void TeamLevelQueue::final()
{
}

bool TeamLevelQueue::tick()
{
	for (auto sz = gTeamCfg->maxTeamPlayer - 1; sz >= 1 && !_allUser.empty(); --sz)
	{
		auto* pSzQueueLv = szQueuePtr(sz);
		if (!pSzQueueLv->teamEmpty() && !_allUser.empty())
		{
			auto* pUser = _allUser.front();
			auto* pTeam = pSzQueueLv->front();
			pUser->cancelMatchPerson();
				//匹配队伍成功
			pTeam->createMember(pUser);
			return true;
		}
	}
	return false;
}

//既有队伍匹配,也有单个玩家在匹配,激活队列
bool TeamLevelQueue::isActive()
{
	if (_allUser.empty()) return false;
	for (auto sz = 1; sz < gTeamCfg->maxTeamPlayer; ++sz)
	{
		if (!szQueueRef(sz).teamEmpty())
			return true;
	}
	return false;
}

TeamSizeQueueLevel* TeamLevelQueue::szQueuePtr(uint32 sz)
{
	return _allSzQueue[gTeamMgr->sizeIndex(sz)];
}

TeamSizeQueueLevel& TeamLevelQueue::szQueueRef(uint32 sz)
{
	return *szQueuePtr(sz);
}

void TeamLevelQueue::addMatchUser(TeamUser* pUser)
{
	assert(pUser != nullptr);
	assert(!pUser->isTeamPersonMatching());
	_allUser.push_back(pUser);
	pUser->lvQueuePair.first = this;
	pUser->lvQueuePair.second = (--_allUser.end());
	//加入激活队列
	gTeamMgr->addLvQueueToActive(this);
}

void TeamLevelQueue::removeMatchUser(TeamUser* pUser)
{
	assert(pUser != nullptr);
	//assert(pUser->isTeamPersonMatching);
	_allUser.erase(pUser->lvQueuePair.second);
	pUser->lvQueuePair.first = nullptr;
	pUser->lvQueuePair.second = _allUser.end();
	//删除激活队列
	gTeamMgr->removeLvQueueFromActive(this);
}

TeamTargetQueue::TeamTargetQueue(const config::team_info_t& cfg) :
	_targetId(cfg.tid)
{
	_allSzQueue.resize(gTeamCfg->maxTeamPlayer - 1, nullptr);
	for (auto sz = 1; sz < gTeamCfg->maxTeamPlayer; ++sz)
	{
		_allSzQueue[TeamMgr::sizeIndex(sz)] = new TeamSizeQueueTarget(sz, this);
	}
	_allLvQueue.resize(cfg.lvMax - cfg.lvMin + 1);
	for (auto lv = cfg.lvMin; lv <= cfg.lvMax; ++lv)
	{
		_allLvQueue[levelIndex(lv)] = new TeamLevelQueue(lv, this);
	}
}

TeamTargetQueue::~TeamTargetQueue()
{
}

void TeamTargetQueue::final()
{
}

bool TeamTargetQueue::validTargetLevel(uint32 lv)
{
	auto* pCfg = gTeamCfg->getTeamConfig(_targetId);
	assert(pCfg != nullptr);
	return lv >= pCfg->lvMin && lv <= pCfg->lvMax;
}

uint32 TeamTargetQueue::levelIndex(uint32 lv)
{
	auto* pCfg = gTeamCfg->getTeamConfig(_targetId);
	assert(pCfg != nullptr);
	return lv - pCfg->lvMin;
}

uint32 TeamTargetQueue::getTargetId()
{
	return _targetId;
}

TeamSizeQueueTarget* TeamTargetQueue::szQueuePtr(uint32 sz)
{
	return _allSzQueue[TeamMgr::sizeIndex(sz)];
}

TeamSizeQueueTarget& TeamTargetQueue::szQueueRef(uint32 sz)
{
	return *szQueuePtr(sz);
}

TeamLevelQueue* TeamTargetQueue::lvQueuePtr(uint32 lv)
{
	return _allLvQueue[levelIndex(lv)];
}

TeamLevelQueue& TeamTargetQueue::lvQueueRef(uint32 lv)
{
	return *lvQueuePtr(lv);
}

void TeamTargetQueue::fill(common::PlatTargetTeamList& out)
{
	out.set_targetid(getTargetId());
	for (auto sz = gTeamCfg->maxTeamPlayer - 1; sz >= 1; --sz)
	{
		auto* pSzQueue = szQueuePtr(sz);
		if (pSzQueue->teamEmpty())
			continue;
		bool full = false;
		pSzQueue->foreachTeam([&](SessionTeam* team)->bool
		{
			if (out.teams_size() >= gParamCfg->teamShowTeamCountMax)
			{
				full = true;
				return false;
			}
			team->fill(*out.add_teams());
			return true;
		});
		if (full)
			break;
	}
}