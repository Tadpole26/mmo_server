#pragma once
#include "sdk.h"
#include "util_arg_opt.h"

class SceneSession;
class FamilyLogic
{
public:
	FamilyLogic() { }
	~FamilyLogic() { }
public:
	bool init();
	bool run();
	void fini();
	void stop();
	void tick(uint32 uiSec);
public:
	SceneSession* getScene(uint32 serverId);
	void addScene(SceneSession* scenesvr);
	void destroyScene(SceneSession *&scenesvr);
public:
	CLogicThread* m_pLogic = nullptr;
	ArgOpt		 m_stArgOpt;						//命令行参数信息
private:
	CSvrLogicFace* m_pInterface = nullptr;
	CSvrLogicFace* m_pClientLIF = nullptr;
private:
	std::unordered_map<uint32, SceneSession*> _allscene;
};

#define gFamilyLogic Singleton<FamilyLogic>::getInstance()