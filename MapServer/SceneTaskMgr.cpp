#include "cLogMgr.h"

#include "SceneTaskMgr.h"
#include "SceneTask.h"

SceneTaskMgr::SceneTaskMgr()
{
}

SceneTaskMgr::~SceneTaskMgr()
{
}

void SceneTaskMgr::final()
{
	_localSceneTaskIndex.clear();
	_groupSceneTaskIndex.clear();
	for (auto& pair : _sceneTaskIndex)
	{
		SAFE_DELETE(pair.second);
	}
	_sceneTaskIndex.clear();
}

SceneTask* SceneTaskMgr::getSceneMapData(uint32 hashId)
{
	auto it = _sceneTaskIndex.find(hashId);
	if (it != _sceneTaskIndex.end()) return it->second;
	auto* data = new SceneTask(hashId);
	auto hashRet = _sceneTaskIndex.emplace(hashId, data);
	if (!hashRet.second)
	{
		SAFE_DELETE(data);
		return nullptr;
	}
	auto eProc = splitEprocHashId(hashId);
	auto eParam = splitEparamHashId(hashId);
	auto funcLocal = ([&]()
		{
			auto& localIndex = _localSceneTaskIndex[eParam];
			auto localRet = localIndex.emplace(eParam, data);
			assert(localRet.second);
		});
	auto funcGroup = ([&]() 
		{
			auto& groupIndex = _groupSceneTaskIndex[eParam];
			auto groupRet = groupIndex.emplace(eParam, data);
			assert(groupRet.second);
		});
	switch (eProc)
	{
	case eProcThreeLocal: funcLocal(); break;
	case eProcThreeGroup: funcGroup(); break;
	default: Log_Error("error proc type.%u", eProc); break;
	}
	return data;
}

void SceneTaskMgr::removeMapDataByScene(uint32 hashId)
{
	auto iter = _sceneTaskIndex.find(hashId);
	if (iter == _sceneTaskIndex.end()) return;
	auto* data = iter->second;
	_sceneTaskIndex.erase(iter);

	auto eProc = splitEprocHashId(hashId);
	auto eParam = splitEparamHashId(hashId);
	auto funcLocal = ([&]() 
		{
			auto iterFind = _localSceneTaskIndex.find(hashId);
			if (iterFind != _localSceneTaskIndex.end())
				_localSceneTaskIndex.erase(iterFind);
		});
	auto funcGroup = ([&]()
		{
			auto iterFind = _groupSceneTaskIndex.find(hashId);
			if (iterFind != _groupSceneTaskIndex.end())
				_groupSceneTaskIndex.erase(iterFind);
		});
	switch (eProc)
	{
	case eProcThreeLocal: funcLocal(); break;
	case eProcThreeGroup: funcGroup(); break;
	default: Log_Error("error proc type.%u", eProc); break;
	}
	SAFE_DELETE(data);
}

void SceneTaskMgr::removeMapData()
{
	for (auto iter = _sceneTaskIndex.begin(); iter != _sceneTaskIndex.end();)
	{
		auto del = iter++;
		auto hashId = del->first;
		removeMapDataByScene(hashId);
	}
}

bool SceneTaskMgr::foreachSceneTaskLocal(uint32 zoneId, SceneTaskVisitFuncT&& func)
{
	auto iter = _localSceneTaskIndex.find(zoneId);
	if (iter == _localSceneTaskIndex.end()) return true;
	return std::all_of(iter->second.begin(), iter->second.end(), [&](const auto& pair) {return func(pair.second); });
}

bool SceneTaskMgr::foreachSceneTaskGroup(uint32 groupId, SceneTaskVisitFuncT&& func)
{
	auto iter = _groupSceneTaskIndex.find(groupId);
	if (iter == _groupSceneTaskIndex.end()) return true;
	return std::all_of(iter->second.begin(), iter->second.end(), [&](const auto& pair) {return func(pair.second); });
}