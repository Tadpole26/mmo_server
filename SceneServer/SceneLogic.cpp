#include "SceneLogic.h"
#include "global_define.h"
#include "zLogMgr.h"
#include "func_proc.h"
#include "global_define.h"
#include "coredump_x.h"
#include "util_file.h"
#include "mongo_base.h"
#include "CDbInstance.h"
#include "ServerNetface.h"
#include <fstream>

bool SceneLogic::Arg(int argc, char* argv[])
{
	return m_stArgOpt.Argv(argc, argv);
}

void SceneLogic::WritePidFile()
{
	std::string strFilePath;
#ifdef _WIN32
	strFilePath = get_exec_path() + "/tmp/";
#else 
	strFilePath = "/tmp/gundam/";
#endif
	create_dir(strFilePath.c_str());
	strFilePath += "_";
	strFilePath += std::to_string(m_oConstCfg.m_uiGroupId);
	strFilePath += ".pid";

	std::ofstream _ofs;
	_ofs.open(strFilePath.c_str(), std::ios_base::trunc | std::ios_base::out);
	_ofs << get_pid() << std::endl;
	_ofs.close();
}

bool SceneLogic::init()
{
#ifdef _WIN32
	Log_Custom("start", "exe: %s ...", get_exec_name().c_str());
#else 
	Log_Custom("start", "exe: %s begin...", get_exec_name().c_str());
#endif
	//����coredump�ļ���core�ص�
	InstallCoreDumper();
	SetConsoleInfo(std::bind(&SceneLogic::stop, this));

	//��ȡ��������
	if (!m_oConstCfg.Init())
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
	if (!GetLogMgrInstance()->Init(m_oConstCfg.m_uiGroupId, m_stArgOpt.GetIndex()))
	{
		Log_Error("log init error!!!");
		return false;
	}
	//��ʼ������zk
	//if (!m_stZkOpt.InitZookeeper(m_oConstCfg.m_strZkHost, m_oConstCfg.m_strNormalConfigPath))
	//{
	//	Log_Error("init zookeeper error !!!");
	//	return false;
	//}

	//��ʼ������mongo
	MongoBase::initialize(m_oConstCfg.m_uiGroupId);
	if (!GAME_DB_INS->Init())
	{
		Log_Error("init mongo db instance error!!!");
		return false;
	}
	
	if (is_listen_port(m_oConstCfg.m_uiGamePort))
	{
		Log_Error("listen port exsit %u!!!", m_oConstCfg.m_uiGamePort);
		return false;
	}

	net_setting stNetConfig;
	stNetConfig.m_nListenPort = m_oConstCfg.m_uiGamePort;
	//�߳�����,ÿ���߳̿���������,����,����,�������С����
	stNetConfig.m_ioThread.Init(1, 4096, ACCEPT_BUF_SIZE, SERVER_BUF_SIZE, SERVER_BUF_SIZE);
	stNetConfig.m_reThread.Init(get_cpu_num(), 20, ACCEPT_BUF_SIZE, SERVER_BUF_SIZE, SERVER_BUF_SIZE);

	m_pLogic = CLogicThread::newm(stNetConfig);
	if (m_pLogic == nullptr)
	{
		Log_Error("logic thread new failed!");
		return false;
	}

	m_pClientLIF = new ClientLogic();
	assert(m_pClientLIF);
	m_pLogic->reg_interface_listen(m_pClientLIF);

	m_pInterface = new CSvrLogicFace();
	assert(m_pInterface);
	m_pLogic->reg_interface_reconn(m_pInterface);

	//m_pSysNetFace = new CSysNetface();
	//m_pSysNetFace->SetServerID(m_oConstCfg.m_uiGroupId);
	//if (m_pLogic->add_reconn(m_pSysNetFace, m_oConstCfg.m_strSysIp, m_oConstCfg.m_uiSysPort,
	//	SERVER_CON_SEC, MAX_SIZE_512M) < 0)
	//	return false;

	//m_pCrossNetFace = new CCrossNetface();
	//m_pCrossNetFace->SetServerID(m_oConstCfg.m_uiGroupId);
	//if (m_pLogic->add_reconn(m_pCrossNetFace, m_oConstCfg.m_strCrossIp, m_oConstCfg.m_uiCrossPort,
	//	SERVER_CON_SEC, MAX_SIZE_512M) < 0)
	//	return false;

	regfn_io_recv_msg(my_io_recv_msg);
	regfn_io_send_msg(my_io_send_msg);
	//m_pUpdateConfigEvent = new CZkEvent(m_pLogic->evthread()->Base(), true, m_stZkOpt.m_pZooHandle, 1000, nullptr);
	//if (m_pUpdateConfigEvent == nullptr)
	//{
	//	Log_Error("zookeeper event add failed!");
	//	return false;
	//}

	//if (!m_pUpdateConfigEvent->init())
	//{
	//	Log_Error("zookeeper event init failed!");
	//	return false;
	//}

	Log_Info("success platid:%u, groupid:%u", m_oConstCfg.m_uiPlatId, m_oConstCfg.m_uiGroupId);
	return true;
}

bool SceneLogic::run()
{
	WritePidFile();
	if (m_pLogic)
	{
		m_pLogic->run();
		return true;
	}
	return false;
}

void SceneLogic::stop()
{
	if (m_pLogic != nullptr)
		m_pLogic->stop();
}

void SceneLogic::fini()
{
	//SAFE_DELETE(m_pSysNetFace);
	//SAFE_DELETE(m_pCrossNetFace);
	SAFE_DELETE(m_pGateSession);
	SAFE_DELETE(m_pInterface);
	SAFE_DELETE(m_pGateSession);
	
	if (m_pLogic)
	{
		CLogicThread::del(m_pLogic);
		m_pLogic = nullptr;
	}
}


