#include "ZoneConstConfig.h"
#include "global_define.h"
#include "util_file.h"
#include "zLogMgr.h"

namespace config
{

	bool ZoneConstConfig::init()
	{
		InitSheet("zone_config", std::bind(&ZoneConstConfig::addCommonCfg, this));
		InitSheet("gate_config", std::bind(&ZoneConstConfig::addGateCfg, this));
		InitSheet("game_config", std::bind(&ZoneConstConfig::addGameCfg, this));
		InitSheet("sys_config", std::bind(&ZoneConstConfig::addSysCfg, this));

		return LoadCfgFromXml(CONST_CONFIG);
	}

	bool ZoneConstConfig::addCommonCfg()
	{
		m_uiGroupId = GetDataUInt("zoneid");
		return true;
	}

	bool ZoneConstConfig::addGateCfg()
	{
		m_strGateIp = GetDataChr("ip");
		m_uiGatePort = GetDataUInt("port");
		m_uiLimit = GetDataUInt("limit");
		m_uiCheckMax = GetDataUInt("check_max");
		m_uiTimeout = GetDataUInt("timeout");
		m_uiWaitCheck = GetDataUInt("wait_check");
		m_uiNotifyInterval = GetDataUInt("notify_interval");
		return true;
	}

	bool ZoneConstConfig::addGameCfg()
	{
		m_strGameIp = GetDataChr("ip");
		m_uiGamePort = GetDataUInt("port");
		return true;
	}

	bool ZoneConstConfig::addSysCfg()
	{
		_familyIp = GetDataChr("ip");
		_familyPort = GetDataUInt("port");
		return true;
	}
}