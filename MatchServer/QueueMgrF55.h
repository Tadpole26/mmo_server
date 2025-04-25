#pragma once

#include "MatchQueueMgr.h"
//段位 人数  某职业数量(职士职业)
class QueueF55;
class QueueTimeF55;
class QueueRobotF55;
class QueueNumCareerF55;
class QueueFormF55;
class QueueMgrF55 : public MatchQueueMgr
{
public:
	enum
	{
		eFirstIdLvId = 1,				//队列类型段位
		eFirstIdTime = 2,				//队列类型时间
		eFirstIdRobot = 3,				//队列类型机器人
	};
	static constexpr uint64 eFirstIdOdd = 100000000000000000;
	static constexpr uint64 eSecondIdOdd = 10000000000000;
	static constexpr uint64 eThirdIdOdd = 100000000000;
	static constexpr uint64 eFourthIdOdd = 1000000000;
public:
	//段位唯一值
	static uint64 unionLvIdKey(uint32 eIdLvId)
	{
		return (uint64)eFirstIdLvId * eFirstIdOdd + (uint64)eIdLvId * eSecondIdOdd;
	}
	static uint32 eIdLvIdByLvIdKey(uint64 key)
	{
		return uint32(key % eFirstIdOdd / eSecondIdOdd);
	}
	//时间类型唯一值
	static uint64 unionTimeKey(uint32 eIdTime)
	{
		return eFirstIdTime * eFirstIdOdd + (uint64)eIdTime * eSecondIdOdd;
	}
	static uint32 eIdTimeByTimeKey(uint64 key)
	{
		return uint32(key % eFirstIdOdd / eSecondIdOdd);
	}
	//机器人
	static uint64 unionRobotKey(uint32 eIdRobot)
	{
		return (uint64)eFirstIdRobot * eFirstIdOdd + (uint64)eIdRobot * eSecondIdOdd;
	}
	static uint32 eIdRobotByRobotKey(uint64 key)
	{
		return uint32(key % eFirstIdOdd / eSecondIdOdd);
	}
	//段位/数量/职业数量
	static uint64 unionNumCareerKey(uint32 eIdLvId, uint32 eIdNum, uint32 eIdCareer)
	{
		return unionLvIdKey(eIdLvId) + (uint64)eIdNum * eThirdIdOdd + (uint64)eIdCareer * eFourthIdOdd;
	}
	static uint32 eIdLvIdByNumCareerKey(uint64 key)
	{
		return eIdLvIdByLvIdKey(key);
	}
	static uint32 eIdNumByNumCareerKey(uint64 key)
	{
		return uint32(key % eSecondIdOdd / eThirdIdOdd);
	}
	static uint32 eIdCareerByNumCareerKey(uint64 key)
	{
		return uint32(key % eSecondIdOdd % eThirdIdOdd / eFourthIdOdd);
	}
	//段位/数量/职业数量/匹配双方阵型
	static uint32 unionFormId(uint32 firstId, uint32 secondId)
	{
		return (firstId << 16) + secondId;
	}
	static uint64 unionFormKey(uint32 eIdLvId, uint32 eIdNum, uint32 eIdCareer, uint32 firstId, uint32 secondId)
	{
		return unionNumCareerKey(eIdLvId, eIdNum, eIdCareer) + unionFormId(firstId, secondId);
	}
	static uint32 eIdLvIdByFormKey(uint64 key)
	{
		return eIdLvIdByLvIdKey(key);
	}
	static uint32 eIdNumByFormKey(uint64 key)
	{
		return uint32(key % eSecondIdOdd / eThirdIdOdd);
	}
	static uint32 eIdCareerByFormKey(uint64 key)
	{
		return uint32(key % eSecondIdOdd % eThirdIdOdd / eFourthIdOdd);
	}
	static uint32 eIdFormIdByFormKey(uint64 key)
	{
		return uint32(key % eSecondIdOdd % eThirdIdOdd % eFourthIdOdd);
	}
public:
	QueueMgrF55(uint64 mgrId);
	virtual ~QueueMgrF55();
	void final() override;
	bool init() override;
public:
	QueueF55* queueF55Ptr(uint64 eid);
	QueueF55& queueF55Ref(uint64 eid);
	QueueTimeF55& eIdTimeRef(uint32 eIdTime);
	QueueRobotF55& eIdRobotRef(uint32 eIdRobot);
	QueueNumCareerF55& eIdNumCareerRef(uint32 eIdLvId, uint32 eIdNum, uint32 eIdCareer);
	QueueFormF55& eIdFormRef(uint32 eIdLvId, uint32 eIdNum, uint32 eIdCareer, uint32 firstId, uint32 secondId);
public:
	uint32 score2LvId(uint32 score);
	uint32 getScoreMax();
	uint32 getIdNumMin();
	uint32 getIdNumMax();
	uint32 getIdLvIdMin();
	uint32 getIdCareerMin();
	uint32 getIdTimeMin();
	uint32 getIdTimeMax();
	uint32 getExtdTimeFirst(uint32 eIdTime);
	uint32 getExtdTimeSecond(uint32 eIdTime);
	uint32 getLvIdScoreOffset();
	uint32 getLvIdExtdOffset();
	uint32 getScoreMin();
	uint32 getIdLvIdMax();
	uint32 getIdRobotMin();
	uint32 getIdRobotMax();
	uint32 getIdCareerMax();
public:
	std::list<uint32> avg2LvIdList(uint32 avg, uint32 extdId);
	std::list<uint32> avg2LvIdListExtdDiff(uint32 avg, uint32 extdId1, uint32 extdId2);
public:
	std::string numCareerSideVec2Str(const TTNumCareerSideT& vec);
	uint32 numCareerSideVec2AutoId(const TTNumCareerSideT& vec);
	uint32 numCareerSideStr2AutoId(const std::string& str);
	const TTNumCareerSideT& numCareerSideStr2Vec(const std::string& str);
	const std::string& numCareerSideAutoId2Str(uint32 autoId);
	const TTNumCareerSideT& numCareerSideAutoId2Vec(uint32 autoId);
	std::list<TTNumCareerSidePairT> numSideVec2NumCareerSideList(const MatchNumSideT& numSide);
	TTNumCareerSidePairT numCareerSideVec2Pair(const TTNumCareerSideT& vec);
	TTNumCareerSideT numCareerSidePair2Vec(const TTNumCareerSidePairT& pair);
public:
	std::list<TTNumCareerFormStrT>& getNumCareerFormListNumAbsoluteCareerAbsolute(uint32 eIdNum, uint32 eIdCareer);
	std::list<TTNumCareerFormStrT>& getNumCareerFormListNumAbsoluteCareerRelative(uint32 eIdNum, uint32 eIdCareer);
	std::list<TTNumCareerFormStrT>& getNumCareerFormListNumRelativeCareerAbsolute(uint32 eIdNum, uint32 eIdCareer);
	std::list<TTNumCareerFormStrT>& getNumCareerFormListNumRelativeCareerRelative(uint32 eIdNum, uint32 eIdCareer);
public:
	const TTTimeVecT& getTimeList();
	const std::list<MatchNumFormStrPVPT>& getNumFormList();
	const std::list<TTNumCareerFormStrT>& getNumCareerFormList();
	const std::list<TTNumCareerFormStrT>& getNumCareerFormListNumAbsoluteCareerAbsolute();
	const std::list<TTNumCareerFormStrT>& getNumCareerFormListNumAbsoluteCareerRelative();
	const std::list<TTNumCareerFormStrT>& getNumCareerFormListNumRelativeCareerAbsolute();
	const std::list<TTNumCareerFormStrT>& getNumCareerFormListNumRelativeCareerRelative();
public:
	MatchNumSideAutoId2StrMapT& getAllNumSideAutoId2Str();
	MatchNumSideAutoId2VecMapT& getAllNumSideAutoId2Vec();
	MatchNumSideStr2AutoIdMapT& getAllNumSideStr2AutoId();
	MatchNumSideStr2VecMapT& getAllNumSideStr2Vec();
public:
	TTNumCareerSideAutoId2StrMapT& getAllNumCareerSideAutoId2Str();
	TTNumCareerSideAutoId2VecMapT& getAllNumCareerSideAutoId2Vec();
	TTNumCareerSideStr2AutoIdMapT& getAllNumCareerSideStr2AutoId();
	TTNumCareerSideStr2VecMapT& getAllNumCareerSideStr2Vec();
};

class F55Kuafu : public QueueMgrF55
{
public:
	F55Kuafu(uint64 mgrId) : QueueMgrF55(mgrId) {}
	virtual ~F55Kuafu() {}
	void final() override { QueueMgrF55::final(); }
};

class F55Group : public QueueMgrF55
{
public:
	F55Group(uint64 mgrId) : QueueMgrF55(mgrId) {}
	virtual ~F55Group() {}
	void final() override { QueueMgrF55::final(); }
};

class F55Local : public QueueMgrF55
{
public:
	F55Local(uint64 mgrId) : QueueMgrF55(mgrId) {}
	virtual ~F55Local() {}
	void final() override { QueueMgrF55::final(); }
};
