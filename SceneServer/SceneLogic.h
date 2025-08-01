#pragma once
#include "singleton.h"
#include "logic_thread.h"
#include "zk_event.h"
#include "CCrossNetface.h"
#include "server_logic_face.h"
#include "CGateSession.h"
#include "util_arg_opt.h"
#include "sdk.h"


class CGateSession;
class CFriendNetface;
class SceneLogic
{
public:
	SceneLogic()  { }
	~SceneLogic() { }
public:
	bool init();
	bool run();
	void fini();
	void stop();
	void OnTimer(uint32 uiSec);
	void WritePidFile();
public:
	bool Arg(int argc, char* argv[]);

public:
	CLogicThread* m_pLogic = nullptr;
	CCrossNetface* m_pCrossNetFace = nullptr;
	CGateSession* m_pGateSession = nullptr;			//这里滚服服务器只有一个gate连接game
	ArgOpt		 m_stArgOpt;						//命令行参数信息
private:
	CSvrLogicFace* m_pInterface = nullptr;
	CSvrLogicFace* m_pClientLIF = nullptr;

};

#define gSceneLogic Singleton<SceneLogic>::getInstance()