#pragma once
#include "MatchDefine.h"

class MatchUnit;
class MatchQueueMgr;
class MatchQueue
{
public:
	MatchQueue(uint64 eid);
	virtual ~MatchQueue();
	virtual void final();
	virtual void tick() {}
	virtual void addUnitFront(MatchUnit* pUnit);
	virtual void addUnit(MatchUnit* pUnit);
	virtual void removeUnit(MatchUnit* pUnit);
	virtual void addToMgr(MatchQueueMgr* mgr);
	virtual MatchUnit* frontAdvance(uint32 offset);
	virtual bool needRemoveFromActive() { return false; }
public:
	uint64 getEid() { return _eid; }
	size_t sizeUnit() { return _units.size(); }
	bool emptyUnit() { return _units.empty(); }
	MatchQueueMgr& mgrRef() { return *_mgr; }
public:
	using UnitListT = std::list<MatchUnit*>;
public:
	MatchQueueActivePairT p;
protected:
	UnitListT _units;							//队列中的匹配单元
private:
	uint64 _eid = std::numeric_limits<uint64>::max();
	MatchQueueMgr* _mgr = nullptr;
};
