#include "SceneTask.h"

#include "cScene.h"
#include "cLogMgr.h"

#include "MapConfig.h"
#include "CopyConfig.h"

SceneTask::SceneTask(uint32 hashId) : _hashId(hashId)
{
}

SceneTask::~SceneTask()
{
}

bool SceneTask::canRegisterStatic()
{
	//只有本区scenesvr可以注册静态地图
	return isSceneSvrLocal(getHashId());
}

bool SceneTask::canRegisterCopy(const uint32 copyLvId)
{
	if (!cScene::isValidCopyLvId(copyLvId))
	{
		Log_Error("canRegisterCopy.!copyLvId.%u", copyLvId);
		return false;
	}
	return !isAutoIdFull(splitEprocHashId(_hashId), splitEparamHashId(_hashId), copyLvId);
}

uint32 SceneTask::getAutoId(const uint32 copyLvId)
{
	return getAutoId(splitEprocHashId(_hashId), splitEparamHashId(_hashId), copyLvId);
}

void SceneTask::putAutoId(const uint32 copyLvId, const uint32 autoId)
{
	return putAutoId(splitEprocHashId(_hashId), splitEparamHashId(_hashId), copyLvId, autoId);
}

uint32 SceneTask::getLineId(const uint32 eProc, const uint32 eParam, const uint32 mapId)
{
	//一个scenesvr进程可能是多个线
	auto unionIndex = unionProcMapIndex(eProc, eParam, mapId);
	auto it = _procStaticLineAllocator.find(unionIndex);
	if (it == _procStaticLineAllocator.end())
	{
		const auto* cfg = gMapCfg->GetMapConfig(mapId);
		if (!cfg)
		{
			Log_Error("getLineId.!cfg:%u", mapId);
			return 0;
		}
		auto* allocator = new cAutoIdAllocator<>(cfg->maxLine);
		auto ret = _procStaticLineAllocator.emplace(unionIndex, allocator);
		if (!ret.second)
		{
			SAFE_DELETE(allocator);
			return 0;
		}
		it = ret.first;
	}
	auto autoId = it->second->alloc();
	if (!it->second->check(autoId))
	{
		Log_Error("getLineId.!check:%u,%u,%u", eProc, eParam, mapId);
		return 0;
	}
	Log_Debug("getLineId.sucess:%u,%u,%u,%u", eProc, eParam, mapId, autoId);
}

void SceneTask::putLineId(const uint32 eProc, const uint32 eParam, const uint32 mapId, const uint32 lineId)
{
	auto unionIndex = unionProcMapIndex(eProc, eParam, mapId);
	auto it = _procStaticLineAllocator.find(unionIndex);
	assert(it != _procStaticLineAllocator.end());
	it->second->free(lineId);
	Log_Debug("putLineId.sucess:%u,%u,%u,%u", eProc, eParam, mapId, lineId)
}

uint32 SceneTask::getAutoId(const uint32 eProc, const uint32 eParam, const uint32 copyLvId)
{
	auto unionIndex = unionProcCopyIndex(eProc, eParam, copyLvId);
	auto it = _procCopyAutoIdAllocator.find(unionIndex);
	if (it == _procCopyAutoIdAllocator.end())
	{
		const auto* cfg = gCopyCfg->getCopyLevelConfig(copyLvId);
		if (!cfg)
		{
			Log_Error("getAutoId.!cfg:%u", copyLvId);
			return 0;
		}
		auto* allocator = new cAutoIdAllocator<>(cScene::AutoIdValMax);
		auto ret = _procCopyAutoIdAllocator.emplace(unionIndex, allocator);
		if (!ret.second)
		{
			SAFE_DELETE(allocator);
			return 0;
		}
		it = ret.first;
	}
	auto autoId = it->second->alloc();
	if (!it->second->check(autoId))
	{
		Log_Error("getAutoId.!check:%u,%u,%u", eProc, eParam, copyLvId);
		return 0;
	}
	Log_Debug("getAutoId.sucess:%u,%u,%u,%u", eProc, eParam, copyLvId, autoId);
}

void SceneTask::putAutoId(const uint32 eProc, const uint32 eParam, const uint32 copyLvId, const uint32 autoId)
{
	auto unionIndex = unionProcCopyIndex(eProc, eParam, copyLvId);
	auto it = _procCopyAutoIdAllocator.find(unionIndex);
	assert(it != _procCopyAutoIdAllocator.end());
	it->second->free(autoId);
	Log_Debug("putAutoId.sucess:%u,%u,%u,%u", eProc, eParam, copyLvId, autoId);
}

bool SceneTask::isAutoIdFull(const uint32 eProc, const uint32 eParam, const uint32 copyLvId)
{
	auto unionIndex = unionProcCopyIndex(eProc, eParam, copyLvId);
	auto it = _procCopyAutoIdAllocator.find(unionIndex);
	if (it == _procCopyAutoIdAllocator.end())
		return false;
	return it->second->isFull();
}