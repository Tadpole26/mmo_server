#include <thread>
#include <fstream>
#include "ZoneConstConfig.h"

#include "CGateLogic.h"
#include "global_define.h"
#include "CGameNetface.h"
#include "CSysNetface.h"
#include "CServerNetface.h"
#include "zLogMgr.h"
#include "util_file.h"
#include "func_proc.h"
#include "coredump_x.h"

CGateLogic::CGateLogic() 
{
	
}

CGateLogic::~CGateLogic() 
{
}

bool CGateLogic::Arg(int argc, char* argv[])
{
	return m_stArgOpt.Argv(argc, argv);
}

void CGateLogic::WritePidFile()
{
	std::string strFilePath;
#ifdef _WIN32
	strFilePath = get_exec_path() + "/tmp/";
#else 
	strFilePath = "/tmp/gundam/";
#endif
	create_dir(strFilePath.c_str());
	strFilePath += "_";
	strFilePath += std::to_string(gZoneCfg->m_uiGroupId);
	strFilePath += ".pid";

	std::ofstream _ofs;
	_ofs.open(strFilePath.c_str(), std::ios_base::trunc | std::ios_base::out);
	_ofs << get_pid() << std::endl;
	_ofs.close();
}

bool CGateLogic::Launch()
{
	if (!Init())
	{
		Stop();
		Fini();
		return false;
	}
	std::string strLocalIp = get_local_ip();
	Log_Custom("start", "local ip:%s", strLocalIp.c_str());
	bool bRet = Run();
	Fini();
	return bRet;
}

bool CGateLogic::Init()
{
#ifdef _WIN32
	Log_Custom("start", "exe: %s ...", get_exec_name().c_str());
#else 
	Log_Custom("start", "exe: %s, index:%d, begin...", get_exec_name().c_str(), GetIndex());
#endif
//����coredump�ļ���core�ص�
	InstallCoreDumper();
	SetConsoleInfo(std::bind(&CGateLogic::Stop, this));

//��ȡ��������
	if (!gZoneCfg->init())
	{
		Log_Error("load const config error!");
		return false;
	}

//������־�ȼ�
#ifdef _WIN32
	GetLogMgrInstance()->SetTargetLevel(eTarConsole, eLevelDebug);
#else 
	GetLogMgrInstance()->SetTargetLevel(eTarConsole, eLevelWarning);
#endif
	GetLogMgrInstance()->SetTargetLevel(eTarFile, eLevelDebug);
	if (!GetLogMgrInstance()->Init(gZoneCfg->m_uiGroupId, m_stArgOpt.GetIndex()))
	{
		Log_Error("log init error!!!");
		return false;
	}

//��������������¼�ѭ��
	net_setting stNetConfig;
	stNetConfig.m_nListenPort = gZoneCfg->m_uiGatePort;
	if (is_listen_port(stNetConfig.m_nListenPort))
	{
		Log_Error("listen port exsit %u!!!", stNetConfig.m_nListenPort);
		return false;
	}
	//����������������
	int iClientLimit = gZoneCfg->m_uiLimit;
	if (iClientLimit < 1024)
		iClientLimit = 1024;

	if (!m_oUserMgr.init())
	{
		Log_Error("playermgr init error size=%d!", iClientLimit);
		return false;
	}

	stNetConfig.m_ioThread.Init(get_cpu_num(), iClientLimit
			, ACCEPT_BUF_SIZE, SERVER_BUF_SIZE, SERVER_BUF_SIZE
			, 512, 120);
	Log_Custom("start", "io thread num:%u, client limit num:%u", get_cpu_num(), iClientLimit);
	stNetConfig.m_reThread.Init(4, 10, ACCEPT_BUF_SIZE
			, SERVER_BUF_SIZE, SERVER_BUF_SIZE);
	Log_Custom("start", "re thread num:%u, client limit num:%u", 4, 10);
	//����������io���߼��߳�
	m_pLogic = CLogicThread::newm(stNetConfig);
	if (m_pLogic == NULL)
	{
		Log_Error("logic thread new failed !!!");
		return false;
	}
	//���������û��ṹ(bufferevent)
	m_pClientLIF = new CClientLogic();
	assert(m_pClientLIF);
	m_pLogic->reg_interface_listen(m_pClientLIF);

	m_pServersLIF = new CSvrLogicFace();
	m_pLogic->reg_interface_reconn(m_pServersLIF);

	m_pGameNetface = new CGameNetface();
	//gateid��groupid��ͬ(һ�����ܶ�Ӧһ��������)
	m_pGameNetface->SetServerID(gZoneCfg->m_uiGroupId);
	ERROR_EXIT(m_pLogic->add_reconn(m_pGameNetface, gZoneCfg->m_strGameIp, gZoneCfg->m_uiGamePort, SERVER_CON_SEC, MAX_SIZE_512M) == 0);

	//ע��read�ص���write�ص�
	regfn_io_recv_msg(my_io_recv_msg);
	regfn_io_send_msg(my_io_send_msg);

	//��ʼ����ʱ����
	m_pUserUpEvent = new CTimeEvent(m_pLogic->evthread()->Base(), true, 100, std::bind(&GateUserMgr::tick, &m_oUserMgr, std::placeholders::_1));
	if (m_pUserUpEvent == nullptr || !m_pUserUpEvent->init())
	{
		Log_Error("init user up time event error !!!");
		return false;
	}

	m_pEvent = new CTimeEvent(m_pLogic->evthread()->Base(), true, 1000, std::bind(&CGateLogic::OnTimer, this, std::placeholders::_1));
	if (m_pEvent == nullptr || !m_pEvent->init())
	{
		Log_Error("init time event error !!!");
		return false;
	}
	return true;
}

bool CGateLogic::Run()
{
	WritePidFile();
	if (m_pLogic) {
		m_pLogic->run();
		return true;
	}
	return false;
}

void CGateLogic::Stop()
{
	if (m_pUserUpEvent != nullptr) 
		m_pUserUpEvent->stop();
	if (m_pLogic != nullptr)
		m_pLogic->stop();
}

void CGateLogic::Fini()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	SAFE_DELETE(m_pUserUpEvent);
	SAFE_DELETE(m_pClientLIF);
	if (m_pLogic)
	{
		CLogicThread::del(m_pLogic);
		m_pLogic = NULL;
	}
	GetLogMgrInstance()->UnInit();
}

void CGateLogic::OnTimer(uint32 dwTm)
{
	//Log_Custom("start", "OnTime %d.......", dwTm);
}

bool CGateLogic::SetMaxSend(uint32 uiSize) 
{ 
	if (uiSize > m_uiMaxSend)
	{
		m_uiMaxSend = uiSize;
		return true;
	}
	return false;
}

