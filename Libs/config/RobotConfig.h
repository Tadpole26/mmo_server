#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct robot_info_t
	{
		uint32 tid = 0;
		std::string nickName;
		uint32 careerId = 0;			//ְҵ
		uint32 actionId = 0;			//��ɫ����
		std::vector<uint32> charId;		//����
		std::vector<uint32> weaponId;	//����id
		uint32 level = 0;				//�ȼ�
		uint32 speed = 0;				//�ƶ��ٶ�
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
