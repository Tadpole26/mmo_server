#include "TeamConfig.h"

namespace config
{
	bool TeamConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("team_info", std::bind(&TeamConfig::AddTeamInfo, this));

		return LoadCfgFromStr(strContent);

	}

	bool TeamConfig::AddTeamInfo()
	{
		uint32 uiUid = GetDataUInt("id");
		team_info_t& stElem = _teamInfoCfg[uiUid];
		return true;
	}

	team_info_t* TeamConfig::getTeamConfig(const uint32 tid)
	{
		auto iter = _teamInfoCfg.find(tid);
		if (iter == _teamInfoCfg.end())
			return nullptr;
		return &(iter->second);
	}

	vote_info_t* TeamConfig::getVoteConfig(const uint32 tid)
	{
		auto iter = _voteInfoCfg.find(tid);
		if (iter == _voteInfoCfg.end())
			return nullptr;
		return &(iter->second);
	}

	bool TeamConfig::foreach(std::function<bool(team_info_t&)> func)
	{
		for (auto& pair : _teamInfoCfg)
		{
			if (!func(pair.second))
				return false;
		}
		return true;
	}
}
