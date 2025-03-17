#pragma once

#include "sdk.h"
#include "cAutoIdAllocator.h"
#include "cServer.h"

class SceneTask
{
public:
	using ProcStaticLineAllocatorT = std::unordered_map<uint64, cAutoIdAllocator<> *>;
	using ProcCopyAutoIdAllocatorT = std::unordered_map<uint64, cAutoIdAllocator<> *>;
public:
	//(zoneId/groupId+copylvid)索引
	static uint64 unionProcCopyIndex(uint32 eProc, uint32 eParam, uint32 copyLvId) { return ((uint64)eProc << 32) + ((uint64)eParam << 16) + (copyLvId & 0xFFFF); }
	//(sceneid+mapid)索引
	static uint64 unionHashMapIndex(uint32 hashId, uint32 mapId) { return ((uint64)hashId << 32) + (mapId & 0xFFFF); }
	//(zoneId/groupId)索引
	static uint32 unionProcIndex(uint32 eProc, uint32 eParam) { return ((uint64)eProc << 16) + (eParam & 0xFFFF); }
	//(zoneId/groupId+mapid)索引
	static uint64 unionProcMapIndex(uint32 eProc, uint32 eParam, uint32 mapId) { return ((uint64)eProc << 32) + ((uint64)eParam << 16) + (mapId & 0xFFFF); }
public:
	SceneTask(uint32 hashId);
	~SceneTask();
public:
	uint32 getHashId() { return _hashId; }
	uint32 getEproc() { return splitEprocHashId(_hashId); }
	uint32 getEparam() { return splitEparamHashId(_hashId); }
	uint32 getOnlineNum() { return _onlinenum; }
	uint32 addOnlineNum() { return ++_onlinenum; }
	uint32 subOnlineNum() { return _onlinenum ? (--_onlinenum) : 0; }
public:
	bool canRegisterStatic();
	bool canRegisterCopy(const uint32 copyLvId);
public:
	uint32 getAutoId(const uint32 copyLvId);
	void putAutoId(const uint32 copyLvId, const uint32 autoId);
	uint32 getLineId(const uint32 eProc, const uint32 eParam, const uint32 mapId);
	void putLineId(const uint32 eProc, const uint32 eParam, const uint32 mapId, const uint32 lineId);
	uint32 getAutoId(const uint32 eProc, const uint32 eParam, const uint32 copyLvId);
	void putAutoId(const uint32 eProc, const uint32 eParam, const uint32 copyLvId, const uint32 autoId);
	bool isAutoIdFull(const uint32 eProc, const uint32 eParam, const uint32 copyLvId);
private:
	ProcStaticLineAllocatorT _procStaticLineAllocator;
	ProcCopyAutoIdAllocatorT _procCopyAutoIdAllocator;
private:
	uint32 _hashId = 0;
	uint32 _onlinenum = 0;
};
