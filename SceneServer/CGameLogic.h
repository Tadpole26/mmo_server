#pragma once
#include "singleton.h"
#include "logic_thread.h"
#include "zk_event.h"
#include "CZkOperation.h"
#include "CConstConfig.h"
#include "CSysNetface.h"
#include "CCrossNetface.h"
#include "server_logic_face.h"
#include "CServerNetface.h"
#include "CGateSession.h"
#include "util_arg_opt.h"


class CGateSession;
class CFriendNetface;
class CGameLogic
{
public:
	CGameLogic()  { }
	~CGameLogic() { }
protected:
	bool Init();
	bool Run();
	void Fini();
	void Stop();
	void OnTimer(uint32 uiSec);
	void WritePidFile();
public:
	bool Arg(int argc, char* argv[]);
	bool Launch();

public:
	CLogicThread* m_pLogic = nullptr;
	CSysNetface* m_pSysNetFace = nullptr;
	CCrossNetface* m_pCrossNetFace = nullptr;
	CGateSession* m_pGateSession = nullptr;			//这里滚服服务器只有一个gate连接game

	CConstConfig  m_oConstCfg;
	ArgOpt		 m_stArgOpt;						//命令行参数信息
public:
	CFriendNetface* GetFriendNetface() { return m_pFriendNetface; }
private:
	bool LoadDataFromMongo();
private:
	CZkEvent*				m_pUpdateConfigEvent;
	ZkOperation				m_stZkOpt;
	CSvrLogicFace* m_pInterface = nullptr;
	CSvrLogicFace* m_pClientLIF = nullptr;
	CFriendNetface* m_pFriendNetface = nullptr;

};

#define GAME_LOGIC_INS Singleton<CGameLogic>::getInstance()