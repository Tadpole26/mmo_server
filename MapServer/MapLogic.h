#pragma once
#include "singleton.h"
#include "logic_thread.h"
#include "server_logic_face.h"
#include "util_arg_opt.h"


class CGateSession;
class MapLogic
{
public:
	MapLogic() { }
	~MapLogic() { }
public:
	bool init();
	bool run();
	void fini();
	void stop();
	void tick(uint32 uiSec);

public:
	CLogicThread* m_pLogic = nullptr;
	ArgOpt		 m_stArgOpt;						//命令行参数信息
private:
	CSvrLogicFace* m_pInterface = nullptr;
	CSvrLogicFace* m_pClientLIF = nullptr;
};

#define gMapLogicIns Singleton<MapLogic>::getInstance()