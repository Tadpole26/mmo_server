#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	class ZoneConstConfig : public cLoadBase
	{
	public:
		bool init();
		bool addCommonCfg();
		bool addGateCfg();
		bool addGameCfg();
		bool addSysCfg();
	public:
		uint32_t					m_uiGroupId = 0;//区服id

		std::string					m_strGateIp = "";
		uint32_t 					m_uiGatePort = 0;
		uint32_t					m_uiLimit = 0;		//允许进游戏人数最大值
		uint32_t					m_uiCheckMax = 0;	//每秒验证的人数上限
		uint32_t					m_uiTimeout = 0;
		uint32_t					m_uiWaitCheck = 0;
		uint32_t					m_uiNotifyInterval = 0;

		std::string					m_strGameIp = "";
		uint32_t					m_uiGamePort = 0;

		std::string					m_strSysIp = "";
		uint32_t					m_uiSysPort = 0;
	};
}

#define gZoneCfg Singleton<config::ZoneConstConfig>::getInstance()
