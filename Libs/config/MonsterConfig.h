#pragma once
#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct monster_group_info_t
	{
		uint32 uiTid = 0;
		uint32 uiMonsterGroupType = 0;						//����������
		bool bAssist = false;								//�ܷ�Эͬ��
		uint32 uiCreateTime = 0;							//����ʱ��
		uint32 uiMaxGroupNum = 0;							//�ܹ���������
		uint32 uiRebornType = 0;							//������������
		uint32 uiRebornNum = 0;								//��������
		uint32 uiRebornCd = 0;								//����ʱ��
		uint32 uiTransType = 0;								//λ������
		std::vector<uint32> vecMonsterIds;					//���������
		uint32 uiRandNum = 0;								//�������������
	};

	struct monster_param_t
	{
		uint32 uiTid = 0;
		uint32 uiMonsterId = 0;								//����id				
		uint32 uiNumber = 0;								//�ù���id���ɵ�����	
		int	iLocationOffsetX = 0;							//��Ա�ˢλ��X
		int iLocationOffsetY = 0;							//��Ա�ˢλ��Y
		int	iRotationOffset = 0;							//��Գ���
	};

	struct monster_info_t
	{
		uint32 uiTid = 0;
		uint32 uiLevel = 0;									//�ȼ�
		uint32 uiMonsterRank = 0;							//����׼�
		uint32 uiAttrId = 0;								//��������id
		uint32 uiDieVanish = 0;								//������ʧʱ��
		std::string strAiName;								//AI�ļ���
		uint32 uiInterval = 0;								//���м��
		uint32 uiSkillId = 0;								//��������
		std::vector<uint32> vecViewRange;					//��Ұ��Χ
		std::vector<uint32> vecAlertRange;					//���䷶Χ
		std::map<uint32, uint32> mapPatrolRadius;			//Ѳ�߸�
		std::vector<uint32> vecPatrolInterval;				//Ѳ�߼��
		uint32 uiPatrolSpeed = 0;							//Ѳ������
		uint32 uiChaseSpeed = 0;							//׷������
		uint32 uiDropId = 0;								//�����id
		uint32 uiPhysique = 0;								//��������
		uint32 uiRace = 0;									//��������
		uint32 uiNature = 0;								//����
		uint32 uiNatureLv = 0;								//���Եȼ�
		uint32 uiAreaRadius = 0;							//ռ�����
		uint32 uiActionTime = 0;							//��������ʱ��
		uint32 uiFallTime = 0;								//����ʱ��
		uint32 uiLeaveTime = 0;								//�뿪ʱ��
		uint32 uiHatchTime = 0;								//����ʱ��
		uint32 uiHatchTid = 0;								//��������
		std::vector<uint32> vecPetCapture;					//����ץ��
	};

	class MonsterConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;
		bool LoadConfigByFile(const std::string& fileName) override;

		monster_group_info_t* GetMonsterGroupConfig(const uint32 uiTid);
		monster_param_t* GetMonsterParamConfig(const uint32 uiTid);
		monster_info_t* GetMonsterInfoConfig(const uint32 uiTid);
	private:
		bool AddMonsterGroupInfo();
		bool AddMonsterParam();
		bool AddMonsterInfo();
	private:
		std::map<uint32, monster_group_info_t> m_mapMonsterGroupInfoCfg;
		std::map<uint32, monster_param_t> m_mapMonsterParamCfg;
		std::map<uint32, monster_info_t> m_mapMonsterInfoCfg;
	};
}
#define gMonsterCfg Singleton<config::MonsterConfig>::getInstance()
