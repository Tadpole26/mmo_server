#include "cMatch.h"

#include "MatchUnit.h"
#include "UnitF55.h"
#include "MatchRoom.h"
#include "MatchUnitMgr.h"
#include "MatchQueueMgr.h"

MatchUnitMgr::MatchUnitMgr()
{
}

MatchUnitMgr::~MatchUnitMgr()
{
}

void MatchUnitMgr::final()
{
	for (auto iter = _alls.begin(); iter != _alls.end();)
	{
		auto del = iter++;
		auto* pUnit = del->second;
		pUnit->finalMe();
	}
}

void MatchUnitMgr::tick()
{
	__recycle();
}

MatchUnit* MatchUnitMgr::createUnit(uint32 zoneId, MatchQueueMgr& mgr)
{
	__autoIdPair();
	zUnitIdType unitId = cMatch::unionUnitId(mgr.getEproc(), (uint32)_idpair.first, _idpair.second);
	MatchUnit* pUnit = nullptr;
	switch (mgr.getMtype())
	{
	case cMatch::eMatchF55: pUnit = new UnitF55(zoneId, unitId, mgr);			break;
	default:				assert(false);										break;
	}
	if (!_alls.emplace(pUnit->getUnitId(), pUnit).second)
	{
		addToRecycle(pUnit);
		return nullptr;
	}
	if (!_zoneIds[zoneId].emplace(pUnit->getUnitId(), pUnit).second)
	{
		addToRecycle(pUnit);
		return nullptr;
	}
	if (!_mgrIds[pUnit->mgrRef().getMgrId()].emplace(pUnit->getUnitId(), pUnit).second)
	{
		addToRecycle(pUnit);
		return nullptr;
	}
	Log_Info("createUnit,%lu", unitId);
	return pUnit;
}

MatchUnit* MatchUnitMgr::getUnit(zUnitIdType unitId)
{
	auto iterFind = _alls.find(unitId);
	if (iterFind != _alls.end())
	{
		return iterFind->second;
	}
	return nullptr;
}

void MatchUnitMgr::addToRecycle(MatchUnit* pUnit)
{
	assert(pUnit != nullptr);
	assert(pUnit->emptyMember());
	assert(pUnit->sizeQueue() == 0);
	assert(pUnit->room == nullptr);
	_alls.erase(pUnit->getUnitId());
	_zoneIds[pUnit->getZoneId()].erase(pUnit->getUnitId());
	_mgrIds[pUnit->mgrRef().getMgrId()].erase(pUnit->getUnitId());
	_recycles.push_back(pUnit);
	Log_Info("addToRecycle,%lu", pUnit->getUnitId());
}

void MatchUnitMgr::__recycle()
{
	if (!_recycles.empty())
	{
		for (auto* pUnit : _recycles)
		{
			assert(pUnit->emptyMember());
			pUnit->final();
			SAFE_DELETE(pUnit);
		}
		_recycles.clear();
	}
}

void MatchUnitMgr::__autoIdPair()
{
	if (_idpair.first != GetCurrTime())
	{
		_idpair.first = GetCurrTime();
		_idpair.second = 0;
	}
	++_idpair.second;
}

bool MatchUnitMgr::__foreach(uint64 mgrId, FuncUnitT func)
{
	auto& units = _mgrIds[mgrId];
	for (auto iter = units.begin(); iter != units.end();)
	{
		auto del = iter++;
		auto* pUnit = del->second;
		if (!func(*pUnit))
		{
			return false;
		}
	}
	return true;
}

void MatchUnitMgr::forceBreakNtf(uint64 mgrId)
{
	Log_Info("forceBreakNtf.%lu", mgrId);
	__foreach(mgrId, [&](auto& unit)->bool
	{
		unit.forceBreakNtf();
		return true;
	});
}

void MatchUnitMgr::evNetDisconnectWithTeam(uint32 zoneId)
{
	Log_Info("evNetDisconnectWithTeam.%u", zoneId);
	std::set<MatchRoom*> roomList;
	auto& units = _zoneIds[zoneId];
	for (auto iter = units.begin(); iter != units.end();)
	{
		auto del = iter++;
		auto* pUnit = del->second;
		if (pUnit->room != nullptr)
		{
			roomList.insert(pUnit->room);
		}
		pUnit->forceBreak();
	}
	/*
	* client::SecondMatch_Ntf_ResetAllPanel ntf;
	* NetInterface::get().sendCmdToWorldLocal(zoneId, client::enClientFirst_Match, client::enSecondMatch_Ntf_ResetAllPanel, ntf);
	*/
	for (auto* pRoom : roomList)
	{
		pRoom->forceBreakNtf();
	}
}