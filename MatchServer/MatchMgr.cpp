#include "cMatch.h"
#include "cServer.h"

#include "CopyConfig.h"

#include "MatchMgr.h"
#include "QueueMgrF55.h"

MatchMgr::MatchMgr()
{
}

MatchMgr::~MatchMgr()
{
}

void MatchMgr::final()
{
	__finalF55();
}

bool MatchMgr::init()
{
	return true;
}

void MatchMgr::tick()
{
	__tickF55();
}

MatchQueueMgr* MatchMgr::createMgr(uint64 mgrId)
{
	auto eMatch = cMatch::splitMtypeMgrId(mgrId);
	auto eProc = cMatch::splitEprocMgrId(mgrId);
	auto eParam = cMatch::splitEparamMgrId(mgrId);
	auto matchId = cMatch::splitMatchIdMgrId(mgrId);
	auto subType = cMatch::splitSubtypeMgrId(mgrId);
	Log_Info("createMgr.%lu.%s,%s.%u,%u,%u", mgrId, cMatch::eMatch2Str(eMatch), eProc2Str(eProc), eParam, matchId, subType);
	auto funcCreateF55 = ([&]()->MatchQueueMgr*
	{
		QueueMgrF55* pMgr = nullptr;
		switch (eProc)
		{
		case eProcThreeLocal: pMgr = new F55Local(mgrId); break;
		case eProcThreeGroup: pMgr = new F55Group(mgrId); break;
		case eProcThreeKuafu: pMgr = new F55Kuafu(mgrId); break;
		}
		if (!_allMgrF55.emplace(mgrId, pMgr).second || !pMgr->init())
		{
			pMgr->final();
			SAFE_DELETE(pMgr);
			return nullptr;
		}
		return pMgr;
	});
	switch (eMatch)
	{
	case cMatch::eMatchF55: return funcCreateF55();		break;
	default: assert(false);								break;
	}
	return nullptr;
}

MatchQueueMgr* MatchMgr::getMgr(uint64 mgrId)
{
	auto eMatch = cMatch::splitMtypeMgrId(mgrId);
	switch (eMatch)
	{
	case cMatch::eMatchF55:			return f55Ptr(mgrId);	break;
	default:						assert(false);			break;
	}
	return nullptr;
}

void MatchMgr::destroyMgr(MatchQueueMgr*& pMgr)
{
	assert(pMgr != nullptr);
	auto mgrId = pMgr->getMgrId();
	auto eMatch = cMatch::splitMtypeMgrId(mgrId);
	auto eProc = cMatch::splitEprocMgrId(mgrId);
	auto eParam = cMatch::splitEparamMgrId(mgrId);
	auto matchId = cMatch::splitMatchIdMgrId(mgrId);
	auto subType = cMatch::splitSubtypeMgrId(mgrId);
	Log_Info("destroyMgr.%lu.%s,%s.%u,%u,%u", mgrId, cMatch::eMatch2Str(eMatch), eProc2Str(eProc), eParam, matchId, subType);
	auto funcDestroyF55 = ([&]() 
	{
		auto iterFind = _allMgrF55.find(mgrId);
		if (iterFind != _allMgrF55.end())
		{
			_allMgrF55.erase(iterFind);
			pMgr->final();
			SAFE_DELETE(pMgr);
		}
	});
	switch (eMatch)
	{
	case cMatch::eMatchF55: funcDestroyF55();	break;
	default:				assert(false);		break;
	}
}

void MatchMgr::forceBreakNtfRecycle(uint32 eProc, uint32 zoneId)
{
}

std::list<uint64> MatchMgr::getMgrIdList(uint32 eMatch, uint32 eProc, uint32 zoneId)
{
	auto groupId = 0;					//通过zoneId获取groupId,TODO
	std::list<uint64> mgrIdList;
	switch (eMatch)
	{
	case cMatch::eMatchF55:
	{
		gCopyCfg->foreachCopyId(eMatch, [&](const auto& cfg)->bool
		{
			switch (eProc)
			{
				case eProcThreeLocal: mgrIdList.push_back(cMatch::unionMgrId(eProc, zoneId, eMatch, cfg.tid));		break;
				case eProcThreeGroup: mgrIdList.push_back(cMatch::unionMgrId(eProc, groupId, eMatch, cfg.tid));		break;
				case eProcThreeKuafu: mgrIdList.push_back(cMatch::unionMgrId(eProc, 0, eMatch, cfg.tid));			break;
			}
			return true;
		});
	}
	break;
	default:
		assert(false);
		break;
	}
	return mgrIdList;
}

QueueMgrF55& MatchMgr::f55Ref(uint32 eProc, uint32 zoneId, uint32 copyId)
{
	auto* ptr = f55Ptr(eProc, zoneId, copyId);
	assert(ptr != nullptr);
	return *ptr;
}

QueueMgrF55* MatchMgr::f55Ptr(uint32 eProc, uint32 zoneId, uint32 copyId)
{
	assert(eProcThreeValid(eProc));
	uint64 mgrId = 0;
	uint32 groupId = 0;					//通过zoneId获取groupId,TODO
	switch (eProc)
	{
	case eProcThreeLocal: mgrId = cMatch::unionMgrId(eProc, zoneId, cMatch::eMatchF55, copyId); break;
	case eProcThreeGroup: mgrId = cMatch::unionMgrId(eProc, groupId, cMatch::eMatchF55, copyId); break;
	case eProcThreeKuafu: mgrId = cMatch::unionMgrId(eProc, 0, cMatch::eMatchF55, copyId); break;
	}
	return f55Ptr(mgrId);
}

QueueMgrF55* MatchMgr::f55Ptr(uint64 mgrId)
{
	auto iterFind = _allMgrF55.find(mgrId);
	if (iterFind == _allMgrF55.end())
	{
		return nullptr;
	}
	return iterFind->second;
}