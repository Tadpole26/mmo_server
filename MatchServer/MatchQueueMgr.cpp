
#include "MatchQueue.h"
#include "MatchQueueMgr.h"
#include "cMatch.h"

MatchQueueMgr::MatchQueueMgr(uint64 mgrId) : 
	_mgrId(mgrId)
{
}

MatchQueueMgr::~MatchQueueMgr()
{
}

void MatchQueueMgr::final()
{
	for (auto& p : _queues)
	{
		p.second->final();
		SAFE_DELETE(p.second);
	}
	_queues.clear();
}

uint32 MatchQueueMgr::szMatchGroupMax()
{
	//return cMatch::eMatch2MatchGroupSize(_matchTid);
	return 5;
}

uint32 MatchQueueMgr::getEproc()
{
	return cMatch::splitEprocMgrId(_mgrId);
}

uint32 MatchQueueMgr::getMatchId()
{
	return cMatch::splitMatchIdMgrId(_mgrId);
}

MatchNumSideVecT MatchQueueMgr::createNumSideList(uint32 szMax)
{
	std::vector<uint32> szSquads(szMax);
	for (uint32 i = 0; i < szMax; ++i)
	{
		szSquads[i] = szMax - i;
	}
	MatchNumSideT formSide;
	MatchNumSideVecT formSideList;
	std::function<void(MatchNumSideT&, uint32, uint32, uint32)> funcCreate;
	funcCreate = ([&](MatchNumSideT& formSide, uint32 sum, uint32 target, uint32 index)
	{
		if (sum == target)
		{
			formSideList.push_back(formSide);
			return;
		}
		if (sum > target || index >= szSquads.size())
			return;

		funcCreate(formSide, sum, target, index + 1);
		formSide.push_back(szSquads[index]);
		funcCreate(formSide, sum + szSquads[index], target, index);
		formSide.pop_back();
	});
	funcCreate(formSide, 0, szMax, 0);
	return formSideList;
}

void MatchQueueMgr::createNumSideListIndex(const MatchNumSideVecT& formSideList)
{
	assert(getAllNumSideAutoId2Str().empty());
	assert(getAllNumSideAutoId2Vec().empty());
	assert(getAllNumSideStr2AutoId().empty());
	assert(getAllNumSideStr2Vec().empty());
	uint32 autoId = 0;
	for (auto iter = formSideList.rbegin(); iter != formSideList.rend(); ++iter)
	{
		++autoId;
		auto& side = *iter;
		auto str = numSideVec2Str(side);
		getAllNumSideStr2Vec().emplace(str, MatchNumSideT(side));
		getAllNumSideStr2AutoId().emplace(str, autoId);
		getAllNumSideAutoId2Str().emplace(autoId, str);
		getAllNumSideAutoId2Vec().emplace(autoId, MatchNumSideT(side));
		Log_Info("NumSize[%s,%u].%u", str.c_str(), autoId, side.size());
	}
}

void MatchQueueMgr::createNumFormList(std::list<MatchNumFormStrPVPT>& formList)
{
	assert(formList.empty());
	auto formSideList = createNumSideList(5);
	createNumSideListIndex(formSideList);
	for (const auto& firstIdList : formSideList)
	{
		for (const auto& secondIdList : formSideList)
		{
			if (firstIdList >= secondIdList)
			{
				auto firstStr = numSideVec2Str(firstIdList);
				auto secondStr = numSideVec2Str(secondIdList);
				auto firstId = numSideStr2AutoId(firstStr);
				auto secondId = numSideStr2AutoId(secondStr);
				formList.emplace_back(MatchNumFormStrPVPT(firstStr, secondStr));
				Log_Debug("NumForm[%spk%s].[%u,%u].%u", firstStr.c_str(), secondStr.c_str(), firstId, secondId, formList.size());
			}
		}
	}
}

void MatchQueueMgr::createNumFormList(std::list<MatchNumFormStrPVET>& formList)
{
	assert(formList.empty());
	auto formSideList = createNumSideList(5);
	createNumSideListIndex(formSideList);
	for (const auto& firstIdList : formSideList)
	{
		auto firstStr = numSideVec2Str(firstIdList);
		auto firstId = numSideStr2AutoId(firstStr);
		formList.emplace_front(firstStr);
		Log_Info("NumForm[%s].[%u].%u", firstStr.c_str(), firstId, formList.size());
	}
}

std::string MatchQueueMgr::numSideVec2Str(const MatchNumSideT& vec)
{
	assert(!vec.empty());
	std::string str;
	for (const auto one : vec)
	{
		str.append(std::to_string(one));
	}
	return str;
}

uint32 MatchQueueMgr::numSideVec2AutoId(const MatchNumSideT& vec)
{
	return numSideStr2AutoId(numSideVec2Str(vec));
}

uint32 MatchQueueMgr::numSideStr2AutoId(const std::string& str)
{
	assert(!str.empty());
	assert(!getAllNumSideStr2AutoId().empty());
	auto iterFind = getAllNumSideStr2AutoId().find(str);
	assert(iterFind != getAllNumSideStr2AutoId().end());
	return iterFind->second;
}

const MatchNumSideT& MatchQueueMgr::numSideStr2Vec(const std::string& str)
{
	assert(!str.empty());
	assert(!getAllNumSideStr2Vec().empty());
	auto iterFind = getAllNumSideStr2Vec().find(str);
	assert(iterFind != getAllNumSideStr2Vec().end());
	return iterFind->second;
}

const std::string& MatchQueueMgr::numSideAutoId2Str(uint32 autoId)
{
	assert(getAllNumSideAutoId2Str().empty());
	auto iterFind = getAllNumSideAutoId2Str().find(autoId);
	assert(iterFind != getAllNumSideAutoId2Str().end());
	return iterFind->second;
}

const MatchNumSideT& MatchQueueMgr::numSideAutoId2Vec(uint32 autoId)
{
	assert(getAllNumSideAutoId2Vec().empty());
	auto iterFind = getAllNumSideAutoId2Vec().find(autoId);
	assert(iterFind != getAllNumSideAutoId2Vec().end());
	return iterFind->second;
}

MatchQueue* MatchQueueMgr::queuePtr(uint64 eid)
{
	auto iter = _queues.find(eid);
	if (iter == _queues.end())
	{
		return nullptr;
	}
	return iter->second;
}

MatchQueue& MatchQueueMgr::queueRef(uint64 eid)
{
	auto* pQueue = queuePtr(eid);
	assert(pQueue != nullptr);
	return *pQueue;
}

void MatchQueueMgr::addQueue(MatchQueue* queue)
{
	assert(queue != nullptr);
	if (!_queues.emplace(queue->getEid(), queue).second)
	{
		assert(false);
	}
}

void MatchQueueMgr::tick()
{
	for (auto iter = _activeList.begin(); iter != _activeList.end();)
	{
		auto del = iter++;
		auto* pQueue = *del;
		pQueue->tick();
		if (pQueue->needRemoveFromActive())
		{
			removeFromActive(pQueue);
			break;
		}
	}
}

void MatchQueueMgr::addToActive(MatchQueue* pQueue)
{
	assert(pQueue != nullptr);
	if (!pQueue->p.first)
	{
		assert(pQueue->sizeUnit());
		_activeList.push_back(pQueue);
		pQueue->p.first = true;
		pQueue->p.second = (--_activeList.end());
		Log_Info("addToAcitve");
	}
}

void MatchQueueMgr::removeFromActive(MatchQueue* pQueue)
{
	assert(pQueue != nullptr);
	if (pQueue->p.first)
	{
		assert(pQueue->emptyUnit());
		_activeList.erase(pQueue->p.second);
		pQueue->p.first = false;
		pQueue->p.second = _activeList.end();
		Log_Info("removeFromActive");
	}
}