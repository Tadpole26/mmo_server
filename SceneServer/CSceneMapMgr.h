#pragma once
#include "sdk.h"
#include "singleton.h"

class CSceneMap;
class CSceneMapMgr
{
public:
	CSceneMapMgr();
	~CSceneMapMgr();
	void Final();
	bool Init();
public:
	const std::map<uint32, std::set<uint32>>& GetAllBrushFiles() { return m_mapAllBrushFiles; }
private:
	bool __ScanAllBrushFiles();
	void __CleanAllBrushFiles();
private:
	std::map<uint32, std::set<uint32>> m_mapAllBrushFiles;
};

#define gSceneMapMgr Singleton<CSceneMapMgr>::getInstance()
