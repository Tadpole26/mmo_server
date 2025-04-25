#include "MatchGroup.h"
#include "MatchUnit.h"
#include "UnitF55.h"

MatchGroup::MatchGroup(uint32 eGroupId) :
	_groupId(eGroupId)
{
	_createTime = GetCurrTime();
}

MatchGroup::~MatchGroup()
{
}

void MatchGroup::addUnit(MatchUnit* pUnit)
{
	assert(pUnit != nullptr);
	_list.push_back(pUnit);
	_size += pUnit->sizeMember();
}

void MatchGroup::foreach(std::function<void(MatchUnit*)> f)
{
	for (auto* pUnit : _list)
	{
		f(pUnit);
	}
}

GroupF55::GroupF55(uint32 eGroupId) :
	MatchGroup(eGroupId)
{
}

GroupF55::~GroupF55()
{
}

void GroupF55::addUnit(MatchUnit* pUnit)
{
	MatchGroup::addUnit(pUnit);
	auto* pUnitF55 = static_cast<UnitF55*>(pUnit);
	_totalMatchScore += pUnitF55->getTotalMatchScore();
	_avgMatchScore = _size > 0 ? _totalMatchScore / _size : 0;

	_totalRealScore += pUnitF55->getTotalRealScore();
	_avgRealScore = _size > 0 ? _totalRealScore / _size : 0;

	if (_createTime < pUnitF55->getCreateTime())
	{
		_createTime = pUnitF55->getCreateTime();
	}
	_ecount += pUnitF55->getECareerCount();
}

const std::list<uint32> GroupF55::friendLvIdList()
{
	return std::list<uint32>();
}

const std::list<uint32> GroupF55::enemyLvIdList()
{
	return std::list<uint32>();
}