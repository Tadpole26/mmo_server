#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct match_info_t
	{
		uint32 tid = 0;
		uint32 firstTime = 0;		//��һ������ʼʱ��
		uint32 lastTime = 0;		//��������ʱ��
		uint32 confirmTime = 0;		//ƥ��ȷ��ʱ��
	};

	struct match_dan_t
	{
		uint32 tid = 0;
		uint32 danUpExp = 0;		//��λ������Ҫ����
		uint32 winAddExp = 0;		//ʤ�����ӻ�������
		uint32 failReduceExp = 0;	//ʧ�ܼ��ٻ�������
		bool isDropDan = 0;			//�Ƿ����λ
		bool isUseWinExp = 0;		//�Ƿ�ʹ�ö�λʤ��
		uint32 danReward;			//��λ����
		uint32 winExp = 0;			//ʤ��ʤ��
		uint32 failExp = 0;			//ʧ��ʤ��
		std::vector<std::vector<uint32>> labelExp;	//ͷ��ʤ��
		uint32 matchingExtend = 1;	//ƥ�������չ
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
