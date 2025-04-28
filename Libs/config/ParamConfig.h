#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	class ParamConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;
	private:
		bool AddParamInfo();
	public:
		bool canMatchRobot(uint32 score);
		uint32 getMinIdRobot();
		uint32 getMaxIdRobot();
		uint32 getMatchRobotWaitTimeMin();
		uint32 getMatchRobotWaitTimeMax();
		uint32 getMatchRobotWaitTimeByCLose(uint32 cLose);
	public:
		uint32 teamInviteExpireSec = 10;				//队伍-邀请到期时间
		uint32 teamApplyExpireSec = 30;					//队伍-申请到期时间
		uint32 teamCreateColdSec = 5;					//队伍-创建冷却
		uint32 teamInviteColdSec = 5;					//队伍-邀请冷却
		uint32 teamReplaceColdSec = 5;					//队伍-顶替冷却
		uint32 teamTogetherColdSec = 5;					//队伍-召集冷却
		uint32 teamAppointColdSec = 5;					//队伍-委任冷却
		uint32 teamApplyColdSec = 1;					//队伍-申请冷却
		uint32 teamQueryListColdSec = 10;				//队伍-查询队伍列表冷却
		uint32 teamApplyCountMax = 30;					//队伍-申请列表上限
		uint32 teamShowTeamCountMax = 30;				//队伍-队伍列表展示上限
		uint32 teamTextMaxLength = 30;					//队伍-招募文本最大长度
		uint32 teamAutoRecruitCold = 10;				//队伍-系统自动招募冷却
		uint32 teamOnceRecruitCount = 3;				//队伍-系统发布招募频道一次条数
		uint32 refusePunishSec = 0;
		uint32 escaprePunishSec = 0;

		uint32 matchRobotMaxScore = 0;
		std::vector<std::pair<uint32, uint32>>	matchRobotWaitTime; //120&60|80&50|60&40|50&30|40&20|15&5
		std::vector<uint32> extdWaitTime{4, 4, 4, 4};
		uint32 lvIdScoreOffset = 0;
		uint32 lvIdExtdOffset = 2;
	};
}
#define gParamCfg Singleton<config::ParamConfig>::getInstance()
