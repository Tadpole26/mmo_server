#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct robot_info_t
	{
		uint32 tid = 0;
		std::string nickName;
		uint32 careerId = 0;			//职业
		uint32 actionId = 0;			//角色动作
		std::vector<uint32> charId;		//外型
		std::vector<uint32> weaponId;	//武器id
		uint32 level = 0;				//等级
		uint32 speed = 0;				//移动速度
	};


	class RobotConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;

		robot_info_t* getRobotConfig(const uint32 tid);
	private:
		bool AddRobotInfo();
	private:
		std::unordered_map<uint32, robot_info_t> _robotInfoCfg;
	};
}
#define gRobotCfg Singleton<config::RobotConfig>::getInstance()
