#include "TeamMgr.h"
#include "SessionTeam.h"
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
		Log_Error("����������Ϊ0");
		return false;
	}
	Log_Info("��ʼ�������������:%u", gTeamCfg->maxTeamPlayer);
	if (!gTeamCfg->foreach([&](config::team_info_t& cfg)->bool
		{
			if (cfg.lvMin == 0)
			{
				Log_Error("��С�ȼ�Ϊ0,%u", cfg.tid);
				return false;
			}
			if (cfg.lvMax == 0)
			{
				Log_Error("���ȼ�Ϊ0,%u", cfg.tid);
				return false;
			}
			if (cfg.lvMax < cfg.lvMin)
			{
				Log_Error("���ȼ�С����С�ȼ�,%u", cfg.tid);
				return false;
			}
			return createTargetQueue(cfg);
		}))
	{
		Log_Error("��ʼ������Ŀ��ʧ��");
		return false;
	}
	Log_Info("��ʼ������Ŀ��ɹ�,%u", sizeTgQueue());
	return true;
}

SessionTeam* TeamMgr::createTeam()
{
	auto* team = new SessionTeam(__createTeamId());
	assert(team != nullptr);
	_allTeam[team->getTeamId()] = team;
	Log_Info("createTeam,%lu", team->getTeamId());
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

void TeamMgr::batLeaveTeam()
{
}

void TeamMgr::batLeaveTeam(std::initializer_list<TeamUser*> roleList, uint32 eLeave)
{
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

zTeamIdType TeamMgr::__createTeamId()
{
	if (_idpair.first != GetCurrTime())
	{
		_idpair.first = GetCurrTime();
		_idpair.second = 1;
	}
	return cTeam::unionTeamId(520, _idpair.first, _idpair.second++);
}