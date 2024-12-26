#pragma once
#include "util_load_base.h"

struct active_skill_t
{
	uint32 uiTid = 0;						//tid ��5λ:������id*100+��������*10+���� ��ɫ6λ:������id*1000+��������*100+�ȼ�
	uint32 uiMainId = 0;						//������id
	uint32 uiStage = 0;						//��:�������� ��ɫ:���ܶ���
	uint32 uiLevel = 0;						//��:���ܶ��� ��ɫ:���ܵȼ�
	uint32 uiChargeStateFix = 0;				//�ɱ�����״̬
	uint32 uiChargeState = 0;				//�̶�����״̬
	uint32 uiCastState = 0;					//�����ͷ�״̬
	uint32 uiWaitState = 0;					//���ܵȴ�״̬
	uint32 uiColdTime = 0;					//����cd
	std::map<uint32, uint32> mapCost;			//��������
	uint32 uiDistance = 1;					//ʩ������(����)
	uint32 uiReleaseType = 0;				//���з�ʽ
	uint32 uiEffectId = 0;					//����Ч��id
	uint32 uiGroup = 0;						//����������

};

struct skill_effective_t
{
	uint32 uiTid = 0;
	std::vector<uint32> vecTarget;			//����id(��Ӧ�������б�)|����id|����id...
	uint32 uiEffectiveId = 0;				//��Ӧ����Ч���߼���id
};

struct skill_effective_logic_t
{
	uint32 uiTid = 0;
	uint32 uiDistanceType = 0;				//1:��ս 2:Զ��
	uint32 uiAttType = 0;					//��������:ˮ���ء��𡢷硢���ȵ�
	uint32 uiDamageType = 0;				//1:���� 2:ħ��
	uint32 uiEffctive = 0;					//Ч��ö��:�˺������ơ�buff�ȵ�
	uint32 uiParma1 = 0;
	uint32 uiParma2 = 0;
	uint32 uiParma3 = 0;
	uint32 uiParma4 = 0;
	uint32 uiSkillType = 0;					//Ч������:������������buff�ȵ�
	bool bTriggerPassive = false;			//�ܷ񴥷�����
};

struct skill_target_t
{
	uint32 uiTid = 0;
	uint32 uiTargetRange = 0;				//�������з�Χ
	uint32 uiShapeType = 0;					//��״����
	std::vector<uint32> vecParams;			//��״��Ӧ����
	uint32 uiGroup = 0;						//��Ӫ
};


class CSkillConfig : public CUtilLoadBase
{
public:
	bool LoadConfig(const std::string& strContent) override;

private:
	bool AddActiveSkill();
	bool AddSkillEffective();
	bool AddSkillEffectiveLogic();
	bool AddSkillTarget();

private:
	std::map<uint32_t, active_skill_t> m_mapActiveSkillCfg;
	std::map<uint32_t, skill_effective_t> m_mapSkillEftCfg;
	std::map<uint32_t, skill_effective_logic_t> m_mapSkillEftLogicCfg;
	std::map<uint32_t, skill_target_t> m_mapSkillTargetCfg;
};

#define gSkillCfg Singleton<CSkillConfig>::getInstance()
