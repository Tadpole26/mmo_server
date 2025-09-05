#include "ZoneConstConfig.h"

#include "FamilyLogic.h"
#include "coredump_x.h"
#include "util_file.h"
#include "func_proc.h"
#include "global_define.h"
#include "zMacro.h"
#include "logic_thread.h"
#include "ServerNetface.h"
#include "SceneSession.h"

bool FamilyLogic::init()
{
#ifdef _WIN32
	Log_Custom("start", "exe: %s ...", get_exec_name().c_str());
#else 
	Log_Custom("start", "exe: %s begin...", get_exec_name().c_str());
#endif
	//����coredump�ļ���core�ص�
	InstallCoreDumper();
	SetConsoleInfo(std::bind(&FamilyLogic::stop, this));
	if (!gZoneCfg->init())
	{
		Log_Error("load zone config error!");
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

	if (is_listen_port(gZoneCfg->getFamilyPort()))
	{
		Log_Error("listen port exsit %u!!!", gZoneCfg->getFamilyPort());
		return false;
	}

	net_setting stNetConfig;
	stNetConfig.m_nListenPort = gZoneCfg->getFamilyPort();
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

	regfn_io_recv_msg(my_io_recv_msg);
	regfn_io_send_msg(my_io_send_msg);
	return true;
}

bool FamilyLogic::run()
{
	if (m_pLogic)
	{
		m_pLogic->run();
		return true;
	}
	return false;
}

void FamilyLogic::stop()
{
	if (m_pLogic != nullptr)
		m_pLogic->stop();
}

void FamilyLogic::fini()
{
	SAFE_DELETE(m_pInterface);
	if (m_pLogic)
	{
		CLogicThread::del(m_pLogic);
		m_pLogic = nullptr;
	}
}

void FamilyLogic::tick(uint32 uiSec)
{
}

SceneSession* FamilyLogic::getScene(uint32 serverId)
{
	auto it = _allscene.find(serverId);
	if (it == _allscene.end()) return nullptr;
	return it->second;
}

void  FamilyLogic::addScene(SceneSession* scenesvr)
{
	_allscene.emplace(scenesvr->GetServerId(), scenesvr);
}

void FamilyLogic::destroyScene(SceneSession*& scenesvr)
{
	_allscene.erase(scenesvr->GetServerId());
}