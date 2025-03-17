#pragma once

#include "sdk.h"
#include "singleton.h"

namespace config
{
	struct copy_level_info_t;
}

class SceneTask;
class SceneTaskMgr
{
public:
	using SceneTaskIndexT = std::unordered_map<uint32, SceneTask*>;
	using LocalSceneTaskIndexT = std::unordered_map<uint32, SceneTaskIndexT>;
	using GroupSceneTaskIndexT = std::unordered_map<uint32, SceneTaskIndexT>;
	using SceneTaskVisitFuncT = std::function<bool(SceneTask*)>;
private:
	SceneTaskIndexT _sceneTaskIndex;
	LocalSceneTaskIndexT _localSceneTaskIndex;
	GroupSceneTaskIndexT _groupSceneTaskIndex;
public:
	SceneTaskMgr();
	~SceneTaskMgr();
	void final();
public:
	SceneTask* getSceneMapData(uint32 hashId);
	void removeMapDataByScene(uint32 hashId);
	void removeMapData();
public:
	bool foreachSceneTaskLocal(uint32 zoneId, SceneTaskVisitFuncT &&func);
	bool foreachSceneTaskGroup(uint32 groupId, SceneTaskVisitFuncT &&func);
public:
	//SceneTask* findCopyTask(uint32 threeId, uint32 copyLvId);
	//SceneTask* findTaskByMatch();
	//SceneTask* findTaskByTeam();
};

#define gSceneTaskMgr Singleton<SceneTaskMgr>::getInstance()