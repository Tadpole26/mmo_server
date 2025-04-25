#include "MatchLogic.h"
#include "zLogMgr.h"
#include "coredump_x.h"
#include "util_file.h"
#include "func_proc.h"
#include "global_define.h"
#include "zMacro.h"
#include "CServerNetface.h"

bool MatchLogic::init()
{
#ifdef _WIN32
	Log_Custom("start", "exe: %s ...", get_exec_name().c_str());
#else 
	Log_Custom("start", "exe: %s begin...", get_exec_name().c_str());
#endif
	//设置coredump文件和core回调
	InstallCoreDumper();
	SetConsoleInfo(std::bind(&MatchLogic::stop, this));

	//设置日志等级
#ifdef _WIN32
	GetLogMgrInstance()->SetTargetLevel(eTarConsole, eLevelDebug);
#else 
	GetLogMgrInstance()->SetTargetLevel(eTarConsole, eLevelWarning);
#endif
	GetLogMgrInstance()->SetTargetLevel(eTarFile, eLevelDebug);
	if (!GetLogMgrInstance()->Init(1, 1))
	{
		Log_Error("log init error!!!");
		return false;
	}

	if (is_listen_port(2600))
	{
		Log_Error("listen port exsit %u!!!", 2600);
		return false;
	}

	net_setting stNetConfig;
	stNetConfig.m_nListenPort = 2600;
	//线程数量,每个线程可连接数量,接受,输入,输出包大小限制
	stNetConfig.m_ioThread.Init(1, 4096, ACCEPT_BUF_SIZE, SERVER_BUF_SIZE, SERVER_BUF_SIZE);
	stNetConfig.m_reThread.Init(get_cpu_num(), 20, ACCEPT_BUF_SIZE, SERVER_BUF_SIZE, SERVER_BUF_SIZE);

	m_pLogic = CLogicThread::newm(stNetConfig);
	if (m_pLogic == nullptr)
	{
		Log_Error("logic thread new failed!");
		return false;
	}

	m_pClientLIF = new CClientLogic();
	assert(m_pClientLIF);
	m_pLogic->reg_interface_listen(m_pClientLIF);

	m_pInterface = new CSvrLogicFace();
	assert(m_pInterface);
	m_pLogic->reg_interface_reconn(m_pInterface);

	regfn_io_recv_msg(my_io_recv_msg);
	regfn_io_send_msg(my_io_send_msg);
	return true;
}

bool MatchLogic::run()
{
	if (m_pLogic)
	{
		m_pLogic->run();
		return true;
	}
	return false;
}

void MatchLogic::stop()
{
	if (m_pLogic != nullptr)
		m_pLogic->stop();
}

void MatchLogic::fini()
{
	SAFE_DELETE(m_pInterface);
	if (m_pLogic)
	{
		CLogicThread::del(m_pLogic);
		m_pLogic = nullptr;
	}
}

void MatchLogic::tick(uint32 uiSec)
{
}