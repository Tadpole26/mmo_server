#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	class ZoneConstConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;
		bool LoadConfigByFile(const std::string& fileName) override;
		bool addCommonCfg();
		bool addGateCfg();
		bool addGameCfg();
		bool addSysCfg();
	public:
		const std::string& getFamilyIp() const{ return _familyIp; }
		uint32_t getFamilyPort() const { return _familyPort; }
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
	private:
		std::string					_familyIp = "";
		uint32_t					_familyPort = 0;
	};
}

#define gZoneCfg Singleton<config::ZoneConstConfig>::getInstance()
