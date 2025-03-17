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
		uint32 teamInviteExpireSec = 10;				//����-���뵽��ʱ��
		uint32 teamApplyExpireSec = 30;					//����-���뵽��ʱ��
		uint32 teamCreateColdSec = 5;					//����-������ȴ
		uint32 teamInviteColdSec = 5;					//����-������ȴ
		uint32 teamReplaceColdSec = 5;					//����-������ȴ
		uint32 teamTogetherColdSec = 5;					//����-�ټ���ȴ
		uint32 teamAppointColdSec = 5;					//����-ί����ȴ
		uint32 teamApplyColdSec = 1;					//����-������ȴ
		uint32 teamQueryListColdSec = 10;				//����-��ѯ�����б���ȴ
		uint32 teamApplyCountMax = 30;					//����-�����б�����
		uint32 teamShowTeamCountMax = 30;				//����-�����б�չʾ����
		uint32 teamTextMaxLength = 30;					//����-��ļ�ı���󳤶�
		uint32 teamAutoRecruitCold = 10;				//����-ϵͳ�Զ���ļ��ȴ
		uint32 teamOnceRecruitCount = 3;				//����-ϵͳ������ļƵ��һ������
	};
}
#define gParamCfg Singleton<config::ParamConfig>::getInstance()
