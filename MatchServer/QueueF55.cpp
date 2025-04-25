#include "cMatch.h"

#include "QueueF55.h"
#include "QueueMgrF55.h"
#include "UnitF55.h"
#include "MatchGroup.h"
#include "MatchRoom.h"
#include "MatchRoomMgr.h"

QueueF55::QueueF55(uint64 key) :
	MatchQueue(key)
{
	p.first = false;
}

QueueF55::~QueueF55()
{
}

void QueueF55::final()
{
	MatchQueue::final();
}

uint32 QueueF55::getIdFirst()
{
	return uint32(getEid() / QueueMgrF55::eFirstIdOdd);
}

uint32 QueueF55::getIdSecond()
{
	return uint32(getEid() % QueueMgrF55::eFirstIdOdd / QueueMgrF55::eSecondIdOdd);
}

QueueMgrF55& QueueF55::mgrF55Ref()
{
	return static_cast<QueueMgrF55&>(mgrRef());
}

bool QueueF55::needRemoveFromActive()
{
	return emptyUnit();
}

//////////////////////////////////////////////////////////////////////////////////////////////
QueueRobotF55::QueueRobotF55(uint32 eIdRobot) :
	QueueF55(QueueMgrF55::unionRobotKey(eIdRobot))
{
}

QueueRobotF55::~QueueRobotF55()
{
}

void QueueRobotF55::addUnit(MatchUnit* pUnit)
{
	MatchQueue::addUnit(pUnit);
	mgrF55Ref().addToActive(this);
}

void QueueRobotF55::tick()
{
}

uint32 QueueRobotF55::getIdRobot()
{
	return QueueMgrF55::eIdRobotByRobotKey(getEid());
}
//////////////////////////////////////////////////////////////////////////////////////////////
QueueTimeF55::QueueTimeF55(uint32 eIdTime) :
	QueueF55(QueueMgrF55::unionTimeKey(eIdTime))
{
}

QueueTimeF55::~QueueTimeF55()
{
}

uint32 QueueTimeF55::getIdTime()
{
	return QueueMgrF55::eIdTimeByTimeKey(getEid());
}

void QueueTimeF55::addUnit(MatchUnit* pUnit)
{
	MatchQueue::addUnit(pUnit);
	mgrF55Ref().addToActive(this);
}

void QueueTimeF55::tick()
{
	for (auto iter = _units.begin(); iter != _units.end();)
	{
		auto del = iter++;
		auto* pUnitF55 = static_cast<UnitF55*>(*del);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
QueueNumCareerF55::QueueNumCareerF55(uint32 eIdLvId, uint32 eIdNum, uint32 eIdCareer) :
	QueueF55(QueueMgrF55::unionNumCareerKey(eIdLvId, eIdNum, eIdCareer))
{
}

QueueNumCareerF55::~QueueNumCareerF55()
{
}

uint32 QueueNumCareerF55::getIdLvId()
{
	return QueueMgrF55::eIdLvIdByNumCareerKey(getEid());
}

uint32 QueueNumCareerF55::getIdNum()
{
	return QueueMgrF55::eIdNumByNumCareerKey(getEid());
}

uint32 QueueNumCareerF55::getIdCareer()
{
	return QueueMgrF55::eIdCareerByNumCareerKey(getEid());
}
//////////////////////////////////////////////////////////////////////////////////////////////
QueueFormF55::QueueFormF55(uint64 key) :
	QueueF55(key)
{
}

QueueFormF55::~QueueFormF55()
{
}

uint32 QueueFormF55::getIdLvId()
{
	return mgrF55Ref().eIdLvIdByFormKey(getEid());
}

uint32 QueueFormF55::getIdNum()
{
	return mgrF55Ref().eIdNumByFormKey(getEid());
}

uint32 QueueFormF55::getIdCarrer()
{
	return mgrF55Ref().eIdCareerByFormKey(getEid());
}

auto funcCheck = ([](QueueMgrF55& mgr, QueueFormF55& queue, const TTNumCareerSidePairT& first, const TTNumCareerSidePairT& second)->bool
{
	std::map<QueueF55*, uint32> offsetMap;
	auto funcMatch = ([&](const TTNumCareerSidePairT& p)->bool
	{
			assert(p.first.size() == p.second.size());
			for (auto i = 0; i < p.first.size(); ++i)
			{
				auto* pQueue = mgr.queueF55Ptr(QueueMgrF55::unionNumCareerKey(queue.getIdLvId(), p.first[i], p.second[i]));
				if (!pQueue) return false;
				auto& offset = offsetMap[pQueue];
				auto* pUnit = pQueue->frontAdvance(offset++);
				if (!pUnit) return false;
			}
			return true;
	});
	return funcMatch(first) && funcMatch(second);
});

void QueueFormF55::addUnit(MatchUnit* pUnit)
{
	MatchQueue::addUnit(pUnit);
	if (!p.first && funcCheck(mgrF55Ref(), *this, first, second))
	{
		mgrF55Ref().addToActive(this);
	}
}

bool QueueFormF55::needRemoveFromActive()
{
	return p.first && !funcCheck(mgrF55Ref(), *this, first, second);
}

void QueueFormF55::tick()
{
	std::map<QueueF55*, uint32> offsetMap;
	while (true)
	{
		auto funcMatch = ([&](GroupF55& group, const TTNumCareerSidePairT& p)->bool
		{
			assert(p.first.size() == p.second.size());
			for (auto i = 0; i < p.first.size(); ++i)
			{
				auto* pQueue = mgrF55Ref().queueF55Ptr(QueueMgrF55::unionNumCareerKey(getIdLvId(), p.first[i], p.second[i]));
				if (!pQueue) return false;
				auto& offset = offsetMap[pQueue];
				auto* pUnit = pQueue->frontAdvance(offset++);
				if (!pUnit) return false;
				group.addUnit(pUnit);
			}
			return true;
		});
		GroupF55 red(cMatch::eRoomGroupIdRed), blue(cMatch::eRoomGroupIdBlue);
		if (funcMatch(red, first) && funcMatch(blue, second))
		{
			std::string strDes = "{" + firstStr + "}vs{" + secondStr + "}";
			auto* pRoom = gMatchRoomMgr->createRoom(mgrF55Ref(), std::move(strDes));
			assert(pRoom != nullptr);
			pRoom->addGroup(red);
			pRoom->addGroup(blue);
		}
		else
		{
			return;
		}
	}
}