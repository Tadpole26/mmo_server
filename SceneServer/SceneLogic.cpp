#include "ZoneConstConfig.h"

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
#include "FamilyNetface.h"
#include "cGame.h"

#include "MapConfig.h"

#include "SceneMapMgr.h"

void SceneLogic::addGateSvr(CGateSession* pGatesvr)
{
	_allgates[pGatesvr->GetServerId()] = pGatesvr;
	Log_Info("addGateSvr.%u", pGatesvr->GetServerId());
}

CGateSession *SceneLogic::getGateSvr(uint32 serverId)
{
	auto it = _allgates.find(serverId);
	if (it == _allgates.end())
	{
		return nullptr;
	}
	return it->second;
}

void SceneLogic::destroyGateSvr(CGateSession* pGatesvr)
{
	auto it = _allgates.find(pGatesvr->GetServerId());
	if (it != _allgates.end())
	{
		_allgates.erase(it);
		SAFE_DELETE(pGatesvr);
	}
}

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
	strFilePath += std::to_string(gZoneCfg->m_uiGroupId);
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
	InstallCoreDumper();
	SetConsoleInfo(std::bind(&SceneLogic::stop, this));
	if (!gZoneCfg->LoadConfigByFile("../Config/zone_const_config.xml"))
	{
		Log_Error("load zone config error!");
		return false;
	}
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
	if (is_listen_port(gZoneCfg->m_uiGamePort))
	{
		Log_Error("listen port exsit %u!!!", gZoneCfg->m_uiGamePort);
		return false;
	}
	cGame::initGlobal();
	if (!loadConfig())
	{
		Log_Error("!loadConfig()");
		return false;
	}
	if (!gSceneMapMgr->init())
	{
		Log_Error("!gSceneMapMgr->init()");
		return false;
	}
	net_setting stNetConfig;
	stNetConfig.m_nListenPort = gZoneCfg->m_uiGamePort;
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

	_familysvr = new FamilyNetface();
	_familysvr->SetServerID(gZoneCfg->m_uiGroupId);
	if (m_pLogic->add_reconn(_familysvr, gZoneCfg->getFamilyIp(), gZoneCfg->getFamilyPort(), SERVER_CON_SEC, MAX_SIZE_512M) < 0)
		return false;

	regfn_io_recv_msg(my_io_recv_msg);
	regfn_io_send_msg(my_io_send_msg);

	Log_Info("success groupid:%u", gZoneCfg->m_uiGroupId);
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
	SAFE_DELETE(m_pInterface);
	
	if (m_pLogic)
	{
		CLogicThread::del(m_pLogic);
		m_pLogic = nullptr;
	}
}

void SceneLogic::OnTimer(uint32 uiSec)
{
}

bool SceneLogic::loadConfig()
{
	if (!gMapCfg->LoadConfigByFile("../Config/game_config/map_config.xml")) return false;
	Log_Info("map size:%u", gMapCfg->sizeMapCfg());
	/*
	auto* pAgent = new CmptMovement();
	auto* pNaveMesh = new NavMeshContext();
	pNaveMesh->load("../Config/map/solo_navmesh.bin");
	Log_Info("寻路开始...");
	if (!pNaveMesh->calculatePath(pAgent, 51.43, -1.82, 5.82))
	{
		return false;
	}
	auto listPath = pNaveMesh->getPath(pAgent);
	for (auto& path : listPath)
	{
		Log_Info("x:%f, y:%f, z:%f", path.getFloatX(), path.getFloatY(), path.getFloatZ());
	}*/
	return true;
}