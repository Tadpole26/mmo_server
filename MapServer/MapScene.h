#pragma once

#include "cScene.h"

class SceneTask;
class MapScene : public cScene
{
public:
	MapScene(const zSceneIdType sceneId);
	virtual ~MapScene();
	virtual bool init();
	virtual void setRegister();
	virtual void setRunning();
	virtual void setRecycle();
	virtual void onOnlineNumChanged(uint32 former) {}
	virtual bool isRoleFull() = 0;
public:
	uint32 getHashId();
	uint32 getOnlineNum() { return _onlinenum; }
	void addOnlineNum();
	void subOnlineNum();
	bool canRoleEnter() { return isRunning() && !isRoleFull(); }
public:
	SceneTask* game = nullptr;
private:
	uint32 _onlinenum = 0;
};
