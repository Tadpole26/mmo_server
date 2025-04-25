#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct robot_info_t
	{
		uint32 tid = 0;
		uint32 type = 0;
		uint32 maxTeamPlayer = 5;
		uint32 minTeamPlayer = 1;
		uint32 lvMin = 0;
		uint32 lvMax = 0;
	};

	struct vote_info_t
	{
		uint32 tid = 0;
		uint32 lastTime = 0;			//ͶƱ����ʱ��
		uint32 confireTime = 0;			//����ȷ��ʱ��
		bool defaultVote = true;		//Ĭ��ͨ��
	};

	struct team_info_t
	{
		uint32 tid = 0;
	};

	class TeamConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;

		team_info_t* getTeamConfig(const uint32 tid);
		vote_info_t* getVoteConfig(const uint32 tid);
		bool foreach(std::function<bool(team_info_t&)> func);
	private:
		bool AddTeamInfo();
	public:
		uint32 maxTeamPlayer = 5;
	private:
		std::unordered_map<uint32, team_info_t> _teamInfoCfg;
		std::unordered_map<uint32, vote_info_t> _voteInfoCfg;
	};
}
#define gTeamCfg Singleton<config::TeamConfig>::getInstance()
