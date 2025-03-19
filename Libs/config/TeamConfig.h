#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct team_info_t
	{
		uint32 tid = 0;
		uint32 type = 0;
		uint32 maxTeamPlayer = 5;
		uint32 minTeamPlayer = 1;
		uint32 lvMin = 0;
		uint32 lvMax = 0;
	};


	class TeamConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;

		team_info_t* getTeamConfig(const uint32 tid);
		bool foreach(std::function<bool(team_info_t&)> func);
	private:
		bool AddTeamInfo();
	public:
		uint32 maxTeamPlayer = 5;
	private:
		std::map<uint32_t, team_info_t> _teamInfoCfg;
	};
}
#define gTeamCfg Singleton<config::TeamConfig>::getInstance()
