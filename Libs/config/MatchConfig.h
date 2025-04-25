#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct match_info_t
	{
		uint32 tid = 0;
		uint32 firstTime = 0;		//第一赛季开始时间
		uint32 lastTime = 0;		//赛季持续时间
		uint32 confirmTime = 0;		//匹配确认时间
	};

	struct match_dan_t
	{
		uint32 tid = 0;
		uint32 danUpExp = 0;		//段位升级需要积分
		uint32 winAddExp = 0;		//胜利增加基础积分
		uint32 failReduceExp = 0;	//失败减少基础积分
		bool isDropDan = 0;			//是否掉段位
		bool isUseWinExp = 0;		//是否使用段位胜点
		uint32 danReward;			//段位奖励
		uint32 winExp = 0;			//胜利胜点
		uint32 failExp = 0;			//失败胜点
		std::vector<std::vector<uint32>> labelExp;	//头衔胜点
		uint32 matchingExtend = 1;	//匹配积分扩展
	};

	class MatchConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;

		match_info_t* getMatchConfig(const uint32 tid);
		match_dan_t* getDanByScore(const uint32 score);
		uint32 getMaxScore() const;
	private:
		bool AddMatchInfo();
	private:
		std::unordered_map<uint32, match_info_t> _matchInfoCfg;
		std::map<uint32, match_dan_t*> _matchDanCfg;
	};
}
#define gMatchCfg Singleton<config::MatchConfig>::getInstance()
