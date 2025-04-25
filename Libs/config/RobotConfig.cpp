#include "RobotConfig.h"

namespace config
{
	bool RobotConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("team_info", std::bind(&RobotConfig::AddRobotInfo, this));

		return LoadCfgFromStr(strContent);

	}

	bool RobotConfig::AddRobotInfo()
	{
		uint32 tid = GetDataUInt("id");
		robot_info_t& stElem = _robotInfoCfg[tid];
		return true;
	}

	robot_info_t* RobotConfig::getRobotConfig(const uint32 tid)
	{
		auto iter = _robotInfoCfg.find(tid);
		if (iter == _robotInfoCfg.end())
			return nullptr;
		return &(iter->second);
	}
}
