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
	void addGateSvr(CGateSession* pGatesvr);
	CGateSession *getGateSvr(uint32 serverId);
	void destroyGateSvr(CGateSession* pGatesvr);
public:
	CLogicThread* m_pLogic = nullptr;
	CCrossNetface* m_pCrossNetFace = nullptr;
	//CGateSession* m_pGateSession = nullptr;			//�������������ֻ��һ��gate����game
	ArgOpt		 m_stArgOpt;						//�����в�����Ϣ
private:
	CSvrLogicFace* m_pInterface = nullptr;
	CSvrLogicFace* m_pClientLIF = nullptr;
private:
	std::unordered_map<uint32, CGateSession*> _allgates;

};

#define gSceneLogic Singleton<SceneLogic>::getInstance()