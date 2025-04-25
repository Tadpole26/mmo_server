#pragma once
#include "sdk.h"

class MatchUnit;
class MatchGroup
{
private:
	uint32 _groupId = 0;
protected:
	std::list<MatchUnit*> _list;
	uint64 _createTime = 0;
	uint32 _size = 0;
public:
	MatchGroup(uint32 eGroupId);
	virtual ~MatchGroup();
	virtual void addUnit(MatchUnit* pUnit);
public:
	uint32 getGroupId() { return _groupId; }
	uint64 getCreateTime() { return _createTime; }
	uint32 sizeMem() { return _size; }
	void foreach(std::function<void(MatchUnit*)> f);
};

class GroupF55 : public MatchGroup
{
protected:
	uint32 _ecount = 0;
	uint32 _totalMatchScore = 0;
	uint32 _avgMatchScore = 0;
	uint32 _totalRealScore = 0;
	uint32 _avgRealScore = 0;
public:
	GroupF55(uint32 eGroupId);
	virtual ~GroupF55();
	void addUnit(MatchUnit* pUnit) override;
public:
	uint32 getTotalMatchScore() { return _totalMatchScore; }
	uint32 getAvgMatchScore() { return _avgMatchScore; }
	uint32 getTotalRealScore() { return _totalRealScore; }
	uint32 getAvgRealScore() { return _avgRealScore; }
	uint32 getECareerCount() { return _ecount; }
	const std::list<uint32> friendLvIdList();
	const std::list<uint32> enemyLvIdList();
};