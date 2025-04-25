#pragma once

#include "sdk.h"
#include "singleton.h"

class MatchQueueMgr;
class QueueMgrF55;
class MatchMgr
{
private:
	std::unordered_map<uint64, QueueMgrF55*> _allMgrF55;
private:
	void __tickF55();
	void __finalF55();
public:
	MatchMgr();
	~MatchMgr();
	void final();
	bool init();
	void tick();
public:
	MatchQueueMgr* createMgr(uint64 mgrId);
	MatchQueueMgr* getMgr(uint64 mgrId);
	void destroyMgr(MatchQueueMgr*& pMgr);
	void forceBreakNtfRecycle(uint32 eProc, uint32 zoneId);
	std::list<uint64> getMgrIdList(uint32 eMatch, uint32 eProc, uint32 zoneId);
public:
	QueueMgrF55& f55Ref(uint32 eProc, uint32 zoneId, uint32 copyId);
	QueueMgrF55* f55Ptr(uint32 eProc, uint32 zoneId, uint32 copyId);
	QueueMgrF55* f55Ptr(uint64 mgrId);
};

#define gMatchMgr Singleton<MatchMgr>::getInstance()


