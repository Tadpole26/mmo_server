
#include "MatchQueue.h"
#include "MatchQueueMgr.h"
#include "MatchUnit.h"

MatchQueue::MatchQueue(uint64 eid) : 
	_eid(eid)
{
}

MatchQueue::~MatchQueue()
{
}

void MatchQueue::final()
{
}

void MatchQueue::addUnitFront(MatchUnit* pUnit)
{
	assert(pUnit != nullptr);
	//assert(pUnit->room == nullptr);
	_units.push_front(pUnit);
	pUnit->addToQueue(this);
}

void MatchQueue::addUnit(MatchUnit* pUnit)
{
	assert(pUnit != nullptr);
	//assert(pUnit->room == nullptr);
	_units.push_back(pUnit);
	pUnit->addToQueue(this);
}

void MatchQueue::removeUnit(MatchUnit* pUnit)
{
	assert(pUnit != nullptr);
	pUnit->removeFromQueue(this);
	auto iter = std::find(_units.begin(), _units.end(), pUnit);
	assert(iter != _units.end());
	_units.erase(iter);
}

void MatchQueue::addToMgr(MatchQueueMgr* mgr)
{
	assert(mgr != nullptr);
	assert(_mgr == nullptr);
	_mgr = mgr;
	_mgr->addQueue(this);
}

MatchUnit* MatchQueue::frontAdvance(uint32 offset)
{
	if (offset == 0)
	{
		return _units.front();
	}
	auto head = _units.begin();
	std::advance(head, offset);
	if (head == _units.end())
		return nullptr;
	return *head;
}
