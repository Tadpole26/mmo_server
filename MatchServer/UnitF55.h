#pragma once

#include "MatchUnit.h"

class UnitF55;
class QueueMgrF55;
typedef void (UnitF55::*TimeFuncPtrF55)();
class UnitF55 : public MatchUnit
{
public:
	UnitF55(const uint32 zoneId, zUnitIdType unitId, MatchQueueMgr& mgr);
	virtual ~UnitF55();
	void final() override;
	void addInitQueue(bool front) override;
	void addMemRole(MemRole* pRole) override;
	void addMemRobot(MemRobot* pRobot) override;
public:
	QueueMgrF55& mgrF55Ref();
	uint32 getIdNum();
	uint32 getIdCareer();
	uint32 getIdRobot();
	bool isGreaterTime();
	void addToTime();
public:
	uint32 getTotalMatchScore() { return _totalMatchScore; }
	uint32 getAvgMatchScore() { return _avgMatchScore; }
	uint32 getTotalRealScore() { return _totalRealScore; }
	uint32 getAvgRealScore() { return _avgRealScore; }
	uint32 getECareerCount() { return _ecareerCount; }
protected:
	void __addMatchScore(uint32 add);
	void __subMatchScore(uint32 sub);
	void __addRealScore(uint32 add);
	void __subRealScore(uint32 sub);
	void __addECareerCount(uint32 careerId);
private:
	void __addToTime1();
	void __addToTime2();
	void __addToTime3();
	void __addToTime4();
	void __addToTime5();
private:
	std::vector<TimeFuncPtrF55> _timeFuncs;
	uint32 _totalMatchScore = 0;						//总匹配分
	uint32 _avgMatchScore = 0;							//平均匹配分
	uint32 _totalRealScore = 0;							//总实际分
	uint32 _avgRealScore = 0;							//平均实际分
	uint32 _ecareerCount = 0;							//排除职业个数
	uint32 _matchRobotTime = std::numeric_limits<uint32>::max();	//匹配机器人时间
	uint32 _extdId = std::numeric_limits<uint32>::max();	//当前扩展Id
	uint32 _extdIdMax = std::numeric_limits<uint32>::max();	//最大扩展Id
	uint32 _extdTime = std::numeric_limits<uint32>::max();	//扩展时间
	uint32 _eIdTime = std::numeric_limits<uint32>::max();	//时间Id
	uint32 _idxId = std::numeric_limits<uint32>::max();	//索引Id
};