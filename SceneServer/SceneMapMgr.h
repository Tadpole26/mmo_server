#pragma once
#include "sdk.h"
#include "singleton.h"

class SceneMap;
class NavMeshContext;
class SceneMapMgr
{
public:
	SceneMapMgr();
	~SceneMapMgr();
	void final();
	bool init();
public:
	const std::map<uint32, std::set<uint32>>& getAllBrushFiles() { return m_mapAllBrushFiles; }
	const NavMeshContext* getNavMesh(uint32 mapId);
public:
	size_t sizeMap() { return _allSceneMap.size(); }
private:
	bool __scanAllBrushFiles();
	void __cleanAllBrushFiles();
private:
	std::unordered_map<uint32, SceneMap*> _allSceneMap;
	std::unordered_map<uint32, NavMeshContext*> _allNavMesh;
	std::map<uint32, std::set<uint32>> m_mapAllBrushFiles;
};

#define gSceneMapMgr Singleton<SceneMapMgr>::getInstance()
