
#include "ParamConfig.h"
#include "MatchConfig.h"

#include "QueueMgrF55.h"
#include "QueueF55.h"

QueueMgrF55::QueueMgrF55(uint64 mgrId) :
	MatchQueueMgr(mgrId)
{
}

QueueMgrF55::~QueueMgrF55()
{
}

void QueueMgrF55::final()
{
	MatchQueueMgr::final();
}

bool QueueMgrF55::init()
{
	const auto& t1 = getNumFormList();
	const auto& t2 = getNumCareerFormList();
	const auto& t3 = getNumCareerFormListNumAbsoluteCareerAbsolute();
	const auto& t4 = getNumCareerFormListNumAbsoluteCareerRelative();
	const auto& t5 = getNumCareerFormListNumRelativeCareerAbsolute();
	const auto& t6 = getNumCareerFormListNumRelativeCareerRelative();
	return true;
}

uint32 QueueMgrF55::score2LvId(uint32 score)
{
	uint32 lvId = std::numeric_limits<uint32>::max();
	if (score <= getScoreMin()) lvId = getIdLvIdMin();
	else if (score >= getScoreMax()) lvId = getIdLvIdMax();
	else if (score < getLvIdScoreOffset()) lvId = getIdLvIdMin();
	else lvId = score / getLvIdScoreOffset();
	return lvId;
}

uint32 QueueMgrF55::getScoreMax()
{
	return getIdLvIdMax() * getLvIdScoreOffset();
}

uint32 QueueMgrF55::getIdNumMin()
{
	return 1;
}

uint32 QueueMgrF55::getIdNumMax()
{
	return szMatchGroupMax();
}

uint32 QueueMgrF55::getIdLvIdMin()
{
	return 1;
}

uint32 QueueMgrF55::getIdCareerMin()
{
	return 0;
}

uint32 QueueMgrF55::getIdTimeMin()
{
	return 1;
}

uint32 QueueMgrF55::getIdTimeMax()
{
	return getTimeList().size();
}

uint32 QueueMgrF55::getExtdTimeFirst(uint32 eIdTime)
{
	const auto& e = getTimeList()[eIdTime - 1];
	return e.first;
}

uint32 QueueMgrF55::getExtdTimeSecond(uint32 eIdTime)
{
	const auto& e = getTimeList()[eIdTime - 1];
	return e.second;
}

uint32 QueueMgrF55::getLvIdScoreOffset()
{
	return gParamCfg->lvIdScoreOffset;
}

uint32 QueueMgrF55::getLvIdExtdOffset()
{
	return gParamCfg->lvIdExtdOffset;
}

uint32 QueueMgrF55::getScoreMin()
{
	return 0;
}

uint32 QueueMgrF55::getIdLvIdMax()
{
	auto max = gMatchCfg->getMaxScore() * 2;
	return max / getLvIdScoreOffset() + 1;
}

uint32 QueueMgrF55::getIdRobotMin()
{
	return gParamCfg->getMinIdRobot();
}

uint32 QueueMgrF55::getIdRobotMax()
{
	return gParamCfg->getMaxIdRobot();
}

uint32 QueueMgrF55::getIdCareerMax()
{
	return 3;
}

std::list<uint32> QueueMgrF55::avg2LvIdList(uint32 avg, uint32 extdId)
{
	auto lvIdOffset = getLvIdExtdOffset() * extdId;
	std::list<uint32> list;
	auto avgLvId = score2LvId(avg);
	list.push_back(avgLvId);
	for (auto i = 1; i <= lvIdOffset; ++i)
	{
		if (getIdLvIdMin() + i <= avgLvId)
		{
			list.push_back(avgLvId - i);
		}
		if (avgLvId + i <= getIdLvIdMax())
		{
			list.push_back(avgLvId + i);
		}
	}
	return list;
}

std::list<uint32> QueueMgrF55::avg2LvIdListExtdDiff(uint32 avg, uint32 extdId1, uint32 extdId2)
{
	if (extdId1 == 0)
	{
		return avg2LvIdList(avg, extdId2);
	}
	auto lvIdOffset1 = getLvIdExtdOffset() * extdId1;
	auto lvIdOffset2 = getLvIdExtdOffset() * extdId2;
	assert(lvIdOffset1 <= lvIdOffset2);
	std::list<uint32> list;
	auto avgLvId = score2LvId(avg);
	for (auto i = lvIdOffset1 + 1; i <= lvIdOffset2; ++i)
	{
		if (getIdLvIdMin() + i <= avgLvId)
		{
			list.push_back(avgLvId - i);
		}
		if (avgLvId + i <= getIdLvIdMax())
		{
			list.push_back(avgLvId + i);
		}
	}
	return list;
}

QueueF55* QueueMgrF55::queueF55Ptr(uint64 eid)
{
	return static_cast<QueueF55*>(queuePtr(eid));
}

QueueF55& QueueMgrF55::queueF55Ref(uint64 eid)
{
	return static_cast<QueueF55&>(queueRef(eid));
}

QueueTimeF55& QueueMgrF55::eIdTimeRef(uint32 eIdTime)
{
	auto key = unionTimeKey(eIdTime);
	auto* pQueue = queueF55Ptr(key);
	if (!pQueue)
	{
		pQueue = new QueueTimeF55(eIdTime);
		pQueue->addToMgr(this);
	}
	assert(pQueue->getIdFirst() == eFirstIdTime);
	assert(pQueue->getIdSecond() == eIdTime);
	auto* pQueueTime = static_cast<QueueTimeF55*>(pQueue);
	assert(pQueueTime->getIdTime() == eIdTime);
	return *pQueueTime;
}

QueueRobotF55& QueueMgrF55::eIdRobotRef(uint32 eIdRobot)
{
	auto key = unionRobotKey(eIdRobot);
	auto* pQueue = queueF55Ptr(key);
	if (!pQueue)
	{
		pQueue = new QueueRobotF55(eIdRobot);
		pQueue->addToMgr(this);
	}
	assert(pQueue->getIdFirst() == eFirstIdRobot);
	assert(pQueue->getIdSecond() == eIdRobot);
	auto* pQueueRobot = static_cast<QueueRobotF55*>(pQueue);
	assert(pQueueRobot->getIdRobot() == eIdRobot);
	return *pQueueRobot;
}

//获取 段位&匹配单元人数&职业数 队列
QueueNumCareerF55& QueueMgrF55::eIdNumCareerRef(uint32 eIdLvId, uint32 eIdNum, uint32 eIdCareer)
{
	auto key = unionNumCareerKey(eIdLvId, eIdNum, eIdCareer);
	auto* pQueue = queueF55Ptr(key);
	if (!pQueue)
	{
		pQueue = new QueueNumCareerF55(eIdLvId, eIdNum, eIdCareer);
		pQueue->addToMgr(this);
	}
	assert(pQueue->getIdFirst() == eFirstIdLvId);
	assert(pQueue->getIdSecond() == eIdLvId);
	auto* pQueueNumCareer = static_cast<QueueNumCareerF55*>(pQueue);
	assert(pQueueNumCareer->getIdLvId() == eIdLvId);
	assert(pQueueNumCareer->getIdNum() == eIdNum);
	assert(pQueueNumCareer->getIdCareer() == eIdCareer);
	return *pQueueNumCareer;
}
//获取 段位&匹配单元人数&职业数 双方阵容 队列
QueueFormF55& QueueMgrF55::eIdFormRef(uint32 eIdLvId, uint32 eIdNum, uint32 eIdCareer, uint32 firstId, uint32 secondId)
{
	eIdNumCareerRef(eIdLvId, eIdNum, eIdCareer);
	auto key = unionFormKey(eIdLvId, eIdNum, eIdCareer, firstId, secondId);
	auto* pQueue = queueF55Ptr(key);
	if (!pQueue)
	{
		auto* pQueue1 = new QueueFormF55(key);
		pQueue1->firstStr = numCareerSideAutoId2Str(firstId);
		pQueue1->secondStr = numCareerSideAutoId2Str(secondId);
		pQueue1->first = numCareerSideVec2Pair(numCareerSideAutoId2Vec(firstId));
		pQueue1->second = numCareerSideVec2Pair(numCareerSideAutoId2Vec(secondId));
		pQueue1->addToMgr(this);
		pQueue = pQueue1;
	}
	assert(pQueue->getIdFirst() == eFirstIdLvId);
	assert(pQueue->getIdSecond() == eIdLvId);
	auto* pQueueForm = static_cast<QueueFormF55*>(pQueue);
	assert(pQueueForm->getIdLvId() == eIdLvId);
	assert(pQueueForm->getIdNum() == eIdNum);
	assert(pQueueForm->getIdCarrer() == eIdCareer);
	return *pQueueForm;
}

std::list<TTNumCareerFormStrT>& QueueMgrF55::getNumCareerFormListNumAbsoluteCareerAbsolute(uint32 eIdNum, uint32 eIdCareer)
{
	static std::list<TTNumCareerFormStrT> formList;
	auto funcFind = ([&](const std::string& str)->bool 
	{
		const auto p = numCareerSideVec2Pair(numCareerSideStr2Vec(str));
		return p.first.end() != std::find(p.first.begin(), p.first.end(), eIdNum) &&
			p.second.end() != std::find(p.second.begin(), p.second.end(), eIdCareer);
	});
	for (const auto& p : getNumCareerFormListNumAbsoluteCareerAbsolute())
	{
		if (funcFind(p.first) || funcFind(p.second))
		{
			if (funcFind(p.first) || funcFind(p.second))
			{
				formList.emplace_back(p);
			}
		}
	}
	return formList;
}

std::list<TTNumCareerFormStrT>& QueueMgrF55::getNumCareerFormListNumAbsoluteCareerRelative(uint32 eIdNum, uint32 eIdCareer)
{
	static std::list<TTNumCareerFormStrT> formList;
	auto funcFind = ([&](const std::string& str)->bool
		{
			const auto p = numCareerSideVec2Pair(numCareerSideStr2Vec(str));
			return p.first.end() != std::find(p.first.begin(), p.first.end(), eIdNum) &&
				p.second.end() != std::find(p.second.begin(), p.second.end(), eIdCareer);
		});
	for (const auto& p : getNumCareerFormListNumAbsoluteCareerRelative())
	{
		if (funcFind(p.first) || funcFind(p.second))
		{
			if (funcFind(p.first) || funcFind(p.second))
			{
				formList.emplace_back(p);
			}
		}
	}
	return formList;
}

std::list<TTNumCareerFormStrT>& QueueMgrF55::getNumCareerFormListNumRelativeCareerAbsolute(uint32 eIdNum, uint32 eIdCareer)
{
	static std::list<TTNumCareerFormStrT> formList;
	auto funcFind = ([&](const std::string& str)->bool
		{
			const auto p = numCareerSideVec2Pair(numCareerSideStr2Vec(str));
			return p.first.end() != std::find(p.first.begin(), p.first.end(), eIdNum) &&
				p.second.end() != std::find(p.second.begin(), p.second.end(), eIdCareer);
		});
	for (const auto& p : getNumCareerFormListNumRelativeCareerAbsolute())
	{
		if (funcFind(p.first) || funcFind(p.second))
		{
			if (funcFind(p.first) || funcFind(p.second))
			{
				formList.emplace_back(p);
			}
		}
	}
	return formList;
}

std::list<TTNumCareerFormStrT>& QueueMgrF55::getNumCareerFormListNumRelativeCareerRelative(uint32 eIdNum, uint32 eIdCareer)
{
	static std::list<TTNumCareerFormStrT> formList;
	auto funcFind = ([&](const std::string& str)->bool
		{
			const auto p = numCareerSideVec2Pair(numCareerSideStr2Vec(str));
			return p.first.end() != std::find(p.first.begin(), p.first.end(), eIdNum) &&
				p.second.end() != std::find(p.second.begin(), p.second.end(), eIdCareer);
		});
	for (const auto& p : getNumCareerFormListNumRelativeCareerRelative())
	{
		if (funcFind(p.first) || funcFind(p.second))
		{
			if (funcFind(p.first) || funcFind(p.second))
			{
				formList.emplace_back(p);
			}
		}
	}
	return formList;
}

//数量职业模版转化成字符串
std::string QueueMgrF55::numCareerSideVec2Str(const TTNumCareerSideT& vec)
{
	std::string str;
	for (const auto one : vec)
	{
		str.append(std::to_string(one));
	}
	return str;
}
//数量职业模版转化成唯一id
uint32 QueueMgrF55::numCareerSideVec2AutoId(const TTNumCareerSideT& vec)
{
	return numCareerSideStr2AutoId(numCareerSideVec2Str(vec));
}
//字符串转化成唯一id
uint32 QueueMgrF55::numCareerSideStr2AutoId(const std::string& str)
{
	assert(!str.empty());
	assert(!getAllNumCareerSideStr2AutoId().empty());
	auto iterFind = getAllNumCareerSideStr2AutoId().find(str);
	assert(iterFind != getAllNumCareerSideStr2AutoId().end());
	return iterFind->second;
}
//字符串转化成数量职业模版
const TTNumCareerSideT& QueueMgrF55::numCareerSideStr2Vec(const std::string& str)
{
	assert(!str.empty());
	assert(!getAllNumCareerSideStr2Vec().empty());
	auto iterFind = getAllNumCareerSideStr2Vec().find(str);
	assert(iterFind != getAllNumCareerSideStr2Vec().end());
	return iterFind->second;
}
//唯一id转换成字符串
const std::string& QueueMgrF55::numCareerSideAutoId2Str(uint32 autoId)
{
	assert(!getAllNumCareerSideAutoId2Str().empty());
	auto iterFind = getAllNumCareerSideAutoId2Str().find(autoId);
	assert(iterFind != getAllNumCareerSideAutoId2Str().end());
	return iterFind->second;
}
//唯一id转换成数量职业模版
const TTNumCareerSideT& QueueMgrF55::numCareerSideAutoId2Vec(uint32 autoId)
{
	assert(!getAllNumCareerSideAutoId2Vec().empty());
	auto iterFind = getAllNumCareerSideAutoId2Vec().find(autoId);
	assert(iterFind != getAllNumCareerSideAutoId2Vec().end());
	return iterFind->second;
}

std::list<TTNumCareerSidePairT> QueueMgrF55::numSideVec2NumCareerSideList(const MatchNumSideT& numSide)
{
	std::function<std::vector<std::vector<uint32>>(const std::vector<uint32>& vec, uint32 index)> func;

	func = [&](const std::vector<uint32>& vec, uint32 index)
		{
			std::vector<std::vector<uint32>> ret;
			if (index == vec.size())
			{
				ret.emplace_back();
				return ret;
			}
			int max = (int)std::min((int)vec[index], 3);
			int min = 0;
			for (auto eIdCareer = max; eIdCareer >= min; eIdCareer--)
			{
				auto tmp = func(vec, index + 1);
				for (auto j = 0; j < tmp.size(); ++j)
				{
					if (index != vec.size() - 1 && vec[index] == vec[index + 1] && !tmp[j].empty() && eIdCareer < tmp[j].front())
					{
						continue;
					}
					ret.emplace_back();
					auto& back = ret.back();
					back.push_back(eIdCareer);
					back.insert(back.end(), tmp[j].begin(), tmp[j].end());
				}
			}
			return ret;
		};
	std::list<TTNumCareerSidePairT> tempList;
	auto tempVec = func(numSide, 0);
	for (const auto& careerVec : tempVec)
	{
		TTNumCareerSidePairT p{ numSide, TTNumCareerSideT(careerVec) };
		tempList.emplace_back(std::move(p));
	}
	return tempList;
}

TTNumCareerSidePairT QueueMgrF55::numCareerSideVec2Pair(const TTNumCareerSideT& vec)
{
	assert(!(vec.size() % 2));
	TTNumCareerSidePairT p;
	for (auto i = 0; i < vec.size(); i += 2)
	{
		p.first.push_back(vec[i]);
		p.second.push_back(vec[i + 1]);
	}
	return p;
}

TTNumCareerSideT QueueMgrF55::numCareerSidePair2Vec(const TTNumCareerSidePairT& pair)
{
	TTNumCareerSideT vec;
	assert(pair.first.size() == pair.second.size());
	for (auto i = 0; i < pair.first.size(); ++i)
	{
		vec.push_back(pair.first[i]);
		vec.push_back(pair.second[i]);
	}
	return vec;
}

const TTTimeVecT& QueueMgrF55::getTimeList()
{
	static TTTimeVecT timeVec;
	if (timeVec.empty())
	{
		uint32 total = 0;
		for (const auto val : gParamCfg->extdWaitTime)
		{
			total += val;
			timeVec.emplace_back(std::pair<uint32, uint32>(val, total));
		}
		auto minRobotTime = gParamCfg->getMatchRobotWaitTimeMin();
		total += minRobotTime;
		timeVec.emplace_back(std::pair<uint32, uint32>(minRobotTime, total));
	}
	return timeVec;
}

const std::list<MatchNumFormStrPVPT>& QueueMgrF55::getNumFormList()
{
	static std::list<MatchNumFormStrPVPT> formList;
	if (formList.empty())
	{
		createNumFormList(formList);
	}
	return formList;
}

const std::list<TTNumCareerFormStrT>& QueueMgrF55::getNumCareerFormList()
{
	static std::list<TTNumCareerFormStrT> formList;
	if (!formList.empty())
	{
		assert(!getAllNumCareerSideAutoId2Str().empty());
		assert(!getAllNumCareerSideAutoId2Vec().empty());
		assert(!getAllNumCareerSideStr2AutoId().empty());
		assert(!getAllNumCareerSideStr2Vec().empty());
		return formList;
	}
	auto funcCreateNumCareerSideList = ([&]()
		{
			auto autoId = 0;
			for (const auto& p : getAllNumSideStr2Vec())
			{
				for (const auto& p1 : numSideVec2NumCareerSideList(p.second))
				{
					++autoId;
					auto side = numCareerSidePair2Vec(p1);
					auto str = numCareerSideVec2Str(side);
					assert(getAllNumCareerSideStr2Vec().emplace(str, TTNumCareerSideT(side)).second);
					assert(getAllNumCareerSideStr2AutoId().emplace(str, autoId).second);
					assert(getAllNumCareerSideAutoId2Str().emplace(autoId, str).second);
					assert(getAllNumCareerSideAutoId2Vec().emplace(autoId, TTNumCareerSideT(side)).second);
					Log_Info("NumCareerSide[%s,%u].%u.(%u,%u,%u,%u)", str.c_str(), autoId, side.size(),
						getAllNumCareerSideStr2Vec().size(), getAllNumCareerSideStr2AutoId().size(),
						getAllNumCareerSideAutoId2Str().size(), getAllNumCareerSideAutoId2Vec().size());
				}
			}
		});

	auto funcCreateNumCareerFormStr = ([&]()
		{
			for (const auto& p1 : getAllNumCareerSideStr2Vec())
			{
				for (const auto& p2 : getAllNumCareerSideStr2Vec())
				{
					if (p1.second >= p2.second)
					{
						const auto& firstStr = p1.first;
						const auto& secondStr = p2.first;
						auto firstId = numCareerSideStr2AutoId(firstStr);
						auto secondId = numCareerSideStr2AutoId(secondStr);
						auto formId = (firstId << 16) + secondId;
						formList.emplace_back(TTNumCareerFormStrT(firstStr, secondStr));
						Log_Info("NumCareerForm[%spk%s].%lu[%u,%u].%u", firstStr.c_str(), secondStr.c_str(),
							formId, firstId, secondId, formList.size());
					}
				}
			}
		});
	funcCreateNumCareerSideList();
	funcCreateNumCareerFormStr();
	return formList;
}

const std::list<TTNumCareerFormStrT>& QueueMgrF55::getNumCareerFormListNumAbsoluteCareerAbsolute()
{
	static std::list<TTNumCareerFormStrT> formList;
	if (!formList.empty())
	{
		return formList;
	}
	for (const auto& p : getNumCareerFormList())
	{
		const auto p1 = numCareerSideVec2Pair(numCareerSideStr2Vec(p.first));
		const auto p2 = numCareerSideVec2Pair(numCareerSideStr2Vec(p.second));
		if (!(p1.first == p2.first && p1.second == p2.second))
		{
			continue;
		}
		const auto& firstStr = p.first;
		const auto& secondStr = p.second;
		auto firstId = numCareerSideStr2AutoId(firstStr);
		auto secondId = numCareerSideStr2AutoId(secondStr);
		auto formId = (firstId << 16) + secondId;
		formList.emplace_back(TTNumCareerFormStrT(p.first, p.second));
		//Log_Info("NumAbsCareerAbs[%spk%s].%lu[%u,%u].%u", firstStr.c_str(), secondStr.c_str(), formId, firstId, secondId, formList.size());
	}
	return formList;
}

const std::list<TTNumCareerFormStrT>& QueueMgrF55::getNumCareerFormListNumAbsoluteCareerRelative()
{
	static std::list<TTNumCareerFormStrT> formList;
	if (!formList.empty())
	{
		return formList;
	}
	for (const auto& p : getNumCareerFormList())
	{
		const auto p1 = numCareerSideVec2Pair(numCareerSideStr2Vec(p.first));
		const auto p2 = numCareerSideVec2Pair(numCareerSideStr2Vec(p.second));
		if (!(p1.first == p2.first && p1.second != p2.second))
		{
			continue;
		}
		const auto& firstStr = p.first;
		const auto& secondStr = p.second;
		auto firstId = numCareerSideStr2AutoId(firstStr);
		auto secondId = numCareerSideStr2AutoId(secondStr);
		auto formId = (firstId << 16) + secondId;
		formList.emplace_back(TTNumCareerFormStrT(p.first, p.second));
		//Log_Info("NumAbsCareerAbs[%spk%s].%lu[%u,%u].%u", firstStr.c_str(), secondStr.c_str(), formId, firstId, secondId, formList.size());
	}
	return formList;
}

const std::list<TTNumCareerFormStrT>& QueueMgrF55::getNumCareerFormListNumRelativeCareerAbsolute()
{
	static std::list<TTNumCareerFormStrT> formList;
	if (!formList.empty())
	{
		return formList;
	}
	for (const auto& p : getNumCareerFormList())
	{
		const auto p1 = numCareerSideVec2Pair(numCareerSideStr2Vec(p.first));
		const auto p2 = numCareerSideVec2Pair(numCareerSideStr2Vec(p.second));
		if (!(p1.first != p2.first && p1.second == p2.second))
		{
			continue;
		}
		const auto& firstStr = p.first;
		const auto& secondStr = p.second;
		auto firstId = numCareerSideStr2AutoId(firstStr);
		auto secondId = numCareerSideStr2AutoId(secondStr);
		auto formId = (firstId << 16) + secondId;
		formList.emplace_back(TTNumCareerFormStrT(p.first, p.second));
		//Log_Info("NumAbsCareerAbs[%spk%s].%lu[%u,%u].%u", firstStr.c_str(), secondStr.c_str(), formId, firstId, secondId, formList.size());
	}
	return formList;
}

const std::list<TTNumCareerFormStrT>& QueueMgrF55::getNumCareerFormListNumRelativeCareerRelative()
{
	static std::list<TTNumCareerFormStrT> formList;
	if (!formList.empty())
	{
		return formList;
	}
	for (const auto& p : getNumCareerFormList())
	{
		const auto p1 = numCareerSideVec2Pair(numCareerSideStr2Vec(p.first));
		const auto p2 = numCareerSideVec2Pair(numCareerSideStr2Vec(p.second));
		if (!(p1.first != p2.first && p1.second != p2.second))
		{
			continue;
		}
		const auto& firstStr = p.first;
		const auto& secondStr = p.second;
		auto firstId = numCareerSideStr2AutoId(firstStr);
		auto secondId = numCareerSideStr2AutoId(secondStr);
		auto formId = (firstId << 16) + secondId;
		formList.emplace_back(TTNumCareerFormStrT(p.first, p.second));
		//Log_Info("NumAbsCareerAbs[%spk%s].%lu[%u,%u].%u", firstStr.c_str(), secondStr.c_str(), formId, firstId, secondId, formList.size());
	}
	return formList;
}

MatchNumSideAutoId2StrMapT& QueueMgrF55::getAllNumSideAutoId2Str()
{
	static MatchNumSideAutoId2StrMapT allmap;
	return allmap;
}

MatchNumSideAutoId2VecMapT& QueueMgrF55::getAllNumSideAutoId2Vec()
{
	static MatchNumSideAutoId2VecMapT allmap;
	return allmap;
}

MatchNumSideStr2AutoIdMapT& QueueMgrF55::getAllNumSideStr2AutoId()
{
	static MatchNumSideStr2AutoIdMapT allmap;
	return allmap;
}

MatchNumSideStr2VecMapT& QueueMgrF55::getAllNumSideStr2Vec()
{
	static MatchNumSideStr2VecMapT allmap;
	return allmap;
}

TTNumCareerSideAutoId2StrMapT& QueueMgrF55::getAllNumCareerSideAutoId2Str()
{
	static TTNumCareerSideAutoId2StrMapT allmap;
	return allmap;
}

TTNumCareerSideAutoId2VecMapT& QueueMgrF55::getAllNumCareerSideAutoId2Vec()
{
	static TTNumCareerSideAutoId2VecMapT allmap;
	return allmap;
}

TTNumCareerSideStr2AutoIdMapT& QueueMgrF55::getAllNumCareerSideStr2AutoId()
{
	static TTNumCareerSideStr2AutoIdMapT allmap;
	return allmap;
}

TTNumCareerSideStr2VecMapT& QueueMgrF55::getAllNumCareerSideStr2Vec()
{
	static TTNumCareerSideStr2VecMapT allmap;
	return allmap;
}
