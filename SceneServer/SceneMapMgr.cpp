#include "SceneMap.h"
#include "SceneMapMgr.h"
#include "NavMeshContext.h"

#include "MapConfig.h"

#include "util_string.h"
#include "util_strop.h"

SceneMapMgr::SceneMapMgr()
{
}

SceneMapMgr::~SceneMapMgr()
{
}

void SceneMapMgr::final()
{
}

bool SceneMapMgr::init()
{
	if (!gMapCfg->foreach([&](auto* pConfig)->bool
		{
			auto* pSceneMap = new SceneMap(pConfig->tid);
			if (!pSceneMap)
			{
				Log_Error("!pSceneMap.%u", pConfig->tid);
				return false;
			}
			if (!pSceneMap->init())
			{
				Log_Error("!pSceneMap->init().%u", pConfig->tid);
				return false;
			}
			if (!_allSceneMap.emplace(pSceneMap->getMapId(), pSceneMap).second)
			{
				Log_Error("!_allSceneMap.emplace.%u", pConfig->tid);
				return false;
			}
			return true;
		}))
	{
		Log_Error("初始化地图失败");
		return false;
	}
	Log_Info("初始化地图成功,共加载地图%u", sizeMap());
	return true;
}

const NavMeshContext* SceneMapMgr::getNavMesh(uint32 mapId)
{
	auto it = _allNavMesh.find(mapId);
	if (it != _allNavMesh.end()) return it->second;
	std::string filePath = "../Config/map/" + std::to_string(mapId) + ".navmesh";
	auto* pNavmeshCtx = new NavMeshContext();
	if (!pNavmeshCtx->load(filePath.c_str()))
	{
		Log_Error("load file %s error", filePath.c_str());
		SAFE_DELETE(pNavmeshCtx);
		return nullptr;
	}
	_allNavMesh[mapId] = pNavmeshCtx;
	return pNavmeshCtx;
}

bool SceneMapMgr::__scanAllBrushFiles()
{
	/*
	std::string strPath = "design/mapcfg/";
	if (!fs::exists(strPath) || !fs::is_directory(strPath))
	{
		Log_Error("%s not exist", strPath.c_str());
		return false;
	}

	fs::recursive_directory_iterator iter(strPath);
	fs::recursive_directory_iterator endIter;
	while (iter != endIter)
	{
		if (fs::is_regular_file(*iter))
		{
			std::string strFileName = iter->path().filename().string();
			std::vector<std::string> vecResult;
			if (str_split(strFileName, ',', vecResult) == 0)
			{
				Log_Error("%s name not valid", strFileName.c_str());
				return false;
			}
			std::vector<uint32> vecFileId;
			if (str_split_num(vecResult[0], ',', vecFileId) != 2)
			{
				Log_Error("%s name not valid", strFileName.c_str());
				return false;
			}
			if (!m_mapAllBrushFiles[vecFileId[0]].insert(vecFileId[1]).second)
			{
				Log_Error("%s name id duplicate", strFileName.c_str());
				return false;
			}
			Log_Info("insert bursh file %s", strFileName.c_str());
		}
	}*/
	return true;
}	

void SceneMapMgr::__cleanAllBrushFiles()
{
	m_mapAllBrushFiles.clear();
}