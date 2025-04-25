
#include "MatchConfig.h"
#include "ParamConfig.h"

#include "UnitF55.h"
#include "MatchMem.h"
#include "MemRoleF55.h"
#include "QueueF55.h"
#include "QueueMgrF55.h"
#include "MatchQueueMgr.h"

UnitF55::UnitF55(const uint32 zoneId, zUnitIdType unitId, MatchQueueMgr& mgr) :
	MatchUnit(zoneId, unitId, mgr),
	_timeFuncs
	{
		&UnitF55::__addToTime1,
		&UnitF55::__addToTime2,
		&UnitF55::__addToTime3,
		&UnitF55::__addToTime4,
		&UnitF55::__addToTime5,
	}
{
}

UnitF55::~UnitF55()
{
}

void UnitF55::final()
{
	MatchUnit::final();
}

QueueMgrF55& UnitF55::mgrF55Ref()
{
	return static_cast<QueueMgrF55&>(mgrRef());
}

void UnitF55::addMemRole(MemRole* pRole)
{
	MatchUnit::addMemRole(pRole);
	//auto* pRoleF55 = static_cast<MemRoleF55*>
}

void UnitF55::addMemRobot(MemRobot* pRobot)
{
}

uint32 UnitF55::getIdNum()
{
	return sizeMember();
}

uint32 UnitF55::getIdCareer()
{
	return getECareerCount();
}

uint32 UnitF55::getIdRobot()
{
	return _matchRobotTime;
}

bool UnitF55::isGreaterTime()
{
	return GetCurrTime() >= _extdTime;
}

void UnitF55::__addMatchScore(uint32 add)
{
	_totalMatchScore += add;
	_avgMatchScore = 0;
	if (sizeMember())
	{
		_avgMatchScore = _totalMatchScore / sizeMember();
	}
}

void UnitF55::__subMatchScore(uint32 sub)
{
	if (sub > _totalMatchScore)
	{
		_totalMatchScore = 0;
		_avgMatchScore = 0;
	}
	else
	{
		_totalMatchScore -= sub;
		_avgMatchScore = 0;
		if (sizeMember())
		{
			_avgMatchScore = _totalMatchScore / sizeMember();
		}
	}
}

void UnitF55::__addRealScore(uint32 add)
{
	_totalRealScore += add;
	_avgRealScore = 0;
	if (sizeMember())
	{
		_avgRealScore = _totalRealScore / sizeMember();
	}
}

void UnitF55::__subRealScore(uint32 sub)
{
	if (sub > _totalRealScore)
	{
		_totalRealScore = 0;
		_avgRealScore = 0;
	}
	else
	{
		_totalRealScore -= sub;
		_avgRealScore = 0;
		if (sizeMember())
		{
			_avgRealScore = _totalRealScore / sizeMember();
		}
	}
}

void UnitF55::__addECareerCount(uint32 careerId)
{
	++_ecareerCount;
}

void UnitF55::addInitQueue(bool front)
{
	MemRoleF55* pLeader = nullptr;
	foreachRole([&](auto& role)->bool 
	{
		if (!role.getLocalLeaderId() || role.getLocalLeaderId() == role.getRoleId())
		{
			pLeader = static_cast<MemRoleF55*>(&role);
			return false;
		}
		return true;
	});
	assert(pLeader != nullptr);
	const auto* cfg = gMatchCfg->getDanByScore(pLeader->getMatchScore());
	assert(cfg != nullptr);
	_idxId = 0;
	_extdId = 1;
	_extdIdMax = std::max((int)cfg->matchingExtend, 1);
	_eIdTime = 1;
	_extdTime = getCreateTime();
	_matchRobotTime = std::numeric_limits<uint32>::max();
	if (gParamCfg->canMatchRobot(pLeader->getMatchScore()))
	{
		_matchRobotTime = gParamCfg->getMatchRobotWaitTimeByCLose(pLeader->getCLoseF55());
	}
	Log_Info("addInitQueue.%u/%u.%u.%u.%u", _extdId, _extdIdMax, pLeader->getMatchScore(), pLeader->getCLoseF55(), _matchRobotTime);
	addToTime();
}

void UnitF55::addToTime()
{
	Log_Info("addToTime%u.%u/%u.%u", _idxId + 1, _extdId, _extdIdMax, _matchRobotTime);
	(this->*_timeFuncs[_idxId])();
}

void UnitF55::__addToTime1()
{
	for (auto eIdLvId : mgrF55Ref().avg2LvIdListExtdDiff(getAvgMatchScore(), _extdId - 1, _extdId))
	{
		mgrF55Ref().eIdNumCareerRef(eIdLvId, getIdNum(), getIdCareer()).addUnit(this);
		for (const auto& p : mgrF55Ref().getNumCareerFormListNumAbsoluteCareerAbsolute(getIdNum(), getIdCareer()))
		{
			auto firstId = mgrF55Ref().numCareerSideStr2AutoId(p.first);
			auto secondId = mgrF55Ref().numCareerSideStr2AutoId(p.second);
			mgrF55Ref().eIdFormRef(eIdLvId, getIdNum(), getIdCareer(), firstId, secondId).addUnit(this);
		}
	}
	mgrF55Ref().eIdTimeRef(_eIdTime).addUnit(this);
}

void UnitF55::__addToTime2()
{
	for (auto eIdLvId : mgrF55Ref().avg2LvIdListExtdDiff(getAvgMatchScore(), _extdId - 1, _extdId))
	{
		for (const auto& p : mgrF55Ref().getNumCareerFormListNumAbsoluteCareerRelative(getIdNum(), getIdCareer()))
		{
			auto firstId = mgrF55Ref().numCareerSideStr2AutoId(p.first);
			auto secondId = mgrF55Ref().numCareerSideStr2AutoId(p.second);
			mgrF55Ref().eIdFormRef(eIdLvId, getIdNum(), getIdCareer(), firstId, secondId).addUnit(this);
		}
	}
	mgrF55Ref().eIdTimeRef(_eIdTime).addUnit(this);
	_extdTime += mgrF55Ref().getExtdTimeFirst(_eIdTime);
	_idxId++;
	_eIdTime++;
}

void UnitF55::__addToTime3()
{
	for (auto eIdLvId : mgrF55Ref().avg2LvIdListExtdDiff(getAvgMatchScore(), _extdId - 1, _extdId))
	{
		for (const auto& p : mgrF55Ref().getNumCareerFormListNumRelativeCareerAbsolute(getIdNum(), getIdCareer()))
		{
			auto firstId = mgrF55Ref().numCareerSideStr2AutoId(p.first);
			auto secondId = mgrF55Ref().numCareerSideStr2AutoId(p.second);
			mgrF55Ref().eIdFormRef(eIdLvId, getIdNum(), getIdCareer(), firstId, secondId).addUnit(this);
		}
	}
	mgrF55Ref().eIdTimeRef(_eIdTime).addUnit(this);
	_extdTime += mgrF55Ref().getExtdTimeFirst(_eIdTime);
	_idxId++;
	_eIdTime++;
}

void UnitF55::__addToTime4()
{
	for (auto eIdLvId : mgrF55Ref().avg2LvIdListExtdDiff(getAvgMatchScore(), _extdId - 1, _extdId))
	{
		for (const auto& p : mgrF55Ref().getNumCareerFormListNumRelativeCareerRelative(getIdNum(), getIdCareer()))
		{
			auto firstId = mgrF55Ref().numCareerSideStr2AutoId(p.first);
			auto secondId = mgrF55Ref().numCareerSideStr2AutoId(p.second);
			mgrF55Ref().eIdFormRef(eIdLvId, getIdNum(), getIdCareer(), firstId, secondId).addUnit(this);
		}
	}
	mgrF55Ref().eIdTimeRef(_eIdTime).addUnit(this);
	_extdTime += mgrF55Ref().getExtdTimeFirst(_eIdTime);
	_idxId++;
	_eIdTime++;
}

void UnitF55::__addToTime5()
{
	mgrF55Ref().eIdRobotRef(getIdRobot()).addUnit(this);
	_extdTime += _matchRobotTime;
}