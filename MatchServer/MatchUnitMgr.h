#pragma once

#include "cMatch.h"
#include "singleton.h"

class MatchUnit;
class MatchQueueMgr;
class MatchUnitMgr
{
public:
	using UnitIdPairT = std::pair<uint64, uint32>;
	using RecycleListT = std::list<MatchUnit*>;
	using UnitMapT = std::unordered_map<zUnitIdType, MatchUnit*>;
	using ZoneIdMapT = std::unordered_map<uint32, UnitMapT>;
	using MgrIdMapT = std::unordered_map<int64, UnitMapT>;
	using FuncUnitT = std::function<bool(MatchUnit&)>;
public:
	MatchUnitMgr();
	~MatchUnitMgr();
	void final();
	void tick();
public:
	void evNetDisconnectWithTeam(uint32 zoneId);
public:
	size_t sizeUnit() { return _alls.size(); }
	size_t sizeRecycle() { return _recycles.size(); }
	MatchUnit* createUnit(uint32 zoneId, MatchQueueMgr& mgr);
	MatchUnit* getUnit(zUnitIdType unitId);
	void addToRecycle(MatchUnit* pUnit);
	void forceBreakNtf(uint64 mgrId);
private:
	void __recycle();
	void __autoIdPair();
	bool __foreach(uint64 mgrId, FuncUnitT func);
private:
	UnitIdPairT _idpair = {0, 0};
	RecycleListT _recycles;
	UnitMapT _alls;
	ZoneIdMapT _zoneIds;
	MgrIdMapT _mgrIds;
};

#define gMatchUnitMgr Singleton<MatchUnitMgr>::getInstance()


