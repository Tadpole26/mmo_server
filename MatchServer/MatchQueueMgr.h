#pragma once

#include "sdk.h"
#include "MatchDefine.h"

class MatchQueue;
class MatchQueueMgr
{
private:
	uint32 _matchTid = 0;																//配置中匹配类型
	uint64 _mgrId;
protected:
	MatchQueueMapT _queues;
	MatchQueueActiveListT _activeList;													//激活队列
public:
	MatchQueueMgr(uint64 mgrId);
	virtual ~MatchQueueMgr();
	virtual void final();
	virtual bool init() = 0;
	virtual void tick();
	virtual void modifyDay() {}
public:
	uint32 szMatchGroupMax();
	MatchNumSideVecT createNumSideList(uint32 szMax);
	void createNumSideListIndex(const MatchNumSideVecT& formSideList);					//创建一边匹配单元数量结构
	void createNumFormList(std::list<MatchNumFormStrPVPT>& formList);
	void createNumFormList(std::list<MatchNumFormStrPVET>& formList);
	std::string numSideVec2Str(const MatchNumSideT& vec);
	uint32 numSideVec2AutoId(const MatchNumSideT& vec);
	uint32 numSideStr2AutoId(const std::string& str);
	const MatchNumSideT& numSideStr2Vec(const std::string& str);
	const std::string& numSideAutoId2Str(uint32 autoId);
	const MatchNumSideT& numSideAutoId2Vec(uint32 autoId);
public:
	void addToActive(MatchQueue* pQueue);
	void removeFromActive(MatchQueue* pQueue);
	size_t sizeActive() { return _activeList.size(); }
public:
	virtual MatchNumSideAutoId2StrMapT& getAllNumSideAutoId2Str() = 0;
	virtual MatchNumSideAutoId2VecMapT& getAllNumSideAutoId2Vec() = 0;
	virtual MatchNumSideStr2AutoIdMapT& getAllNumSideStr2AutoId() = 0;
	virtual MatchNumSideStr2VecMapT& getAllNumSideStr2Vec() = 0;
public:
	uint32 getMtype() { return _matchTid; }
	uint32 getEproc();
	//uint32 getEparam();
	uint32 getMatchId();
	uint64 getMgrId() { return _mgrId; }
	size_t sizeQueue() { return _queues.size(); }
	MatchQueue* queuePtr(uint64 eid);
	MatchQueue& queueRef(uint64 eid);
	void addQueue(MatchQueue* queue);
};
