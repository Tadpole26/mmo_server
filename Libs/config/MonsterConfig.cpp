#include "MonsterConfig.h"

namespace config
{
	bool MonsterConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("monster_group_info", std::bind(&MonsterConfig::AddMonsterGroupInfo, this));
		InitSheet("monster_param", std::bind(&MonsterConfig::AddMonsterParam, this));
		InitSheet("monster_info", std::bind(&MonsterConfig::AddMonsterInfo, this));
		return LoadCfgFromStr(strContent);

	}

	bool MonsterConfig::AddMonsterGroupInfo()
	{
		uint32 uiTid = GetDataUInt("id");
		monster_group_info_t& stElem = m_mapMonsterGroupInfoCfg[uiTid];
		stElem.uiTid = uiTid;
		stElem.uiMonsterGroupType = GetDataUInt("monster_group_type");							//����������
		stElem.bAssist = GetDataBool("is_assist");												//�ܷ�Эͬ��
		stElem.uiCreateTime = GetDataUInt("create_time");										//����ʱ��
		stElem.uiMaxGroupNum = GetDataUInt("max_group_num");									//�ܹ���������
		stElem.uiRebornType = GetDataUInt("reborn_type");										//������������
		stElem.uiRebornNum = GetDataUInt("reborn_num");											//��������
		stElem.uiRebornCd = GetDataUInt("reborn_cd");											//����ʱ��
		stElem.uiTransType = GetDataUInt("trans_type");											//λ������
		GetDataVec("monsters", stElem.vecMonsterIds);											//���������
		stElem.uiRandNum = GetDataUInt("rand_num");												//�������������
		return true;
	}

	bool MonsterConfig::AddMonsterParam()
	{
		uint32 uiTid = GetDataUInt("id");
		monster_param_t& stElem = m_mapMonsterParamCfg[uiTid];
		stElem.uiTid = uiTid;
		stElem.uiMonsterId = GetDataUInt("monster_id");
		stElem.uiNumber = GetDataUInt("number");
		stElem.iLocationOffsetX = GetDataInt("location_offset_x");
		stElem.iLocationOffsetY = GetDataInt("location_offset_y");
		stElem.iRotationOffset = GetDataInt("roration_offset");
		return true;
	}

	bool MonsterConfig::AddMonsterInfo()
	{
		uint32 uiTid = GetDataUInt("id");
		monster_info_t& stElem = m_mapMonsterInfoCfg[uiTid];
		stElem.uiLevel = GetDataUInt("level");
		stElem.uiMonsterRank = GetDataUInt("monster_rank");
		stElem.uiAttrId = GetDataUInt("attr_id");
		stElem.uiDieVanish = GetDataUInt("die_vanish");
		stElem.strAiName = GetDataChr("ai_name");
		stElem.uiInterval = GetDataUInt("insterval");
		stElem.uiSkillId = GetDataUInt("skill_id");
		GetDataVec("view_range", stElem.vecViewRange);
		GetDataVec("alert_range", stElem.vecAlertRange);
		//stElem.mapPatrolRadius;			//Ѳ�߸�
		GetDataVec("patrol_interval", stElem.vecPatrolInterval);
		stElem.uiPatrolSpeed = GetDataUInt("patrol_speed");
		stElem.uiChaseSpeed = GetDataUInt("chase_speed");
		stElem.uiDropId = GetDataUInt("drop_id");
		stElem.uiPhysique = GetDataUInt("physique");
		stElem.uiRace = GetDataUInt("race");
		stElem.uiNature = GetDataUInt("nature");
		stElem.uiNatureLv = GetDataUInt("nature_lv");
		stElem.uiAreaRadius = GetDataUInt("area_radius");
		stElem.uiActionTime = GetDataUInt("action_time");
		stElem.uiFallTime = GetDataUInt("fall_time");
		stElem.uiLeaveTime = GetDataUInt("leave_time");
		stElem.uiHatchTime = GetDataUInt("hatch_time");
		stElem.uiHatchTid = GetDataUInt("hatch_tid");
		GetDataVec("pet_capture", stElem.vecPetCapture);
		return true;
	}

	monster_group_info_t* MonsterConfig::GetMonsterGroupConfig(const uint32 uiTid)
	{
		auto iter = m_mapMonsterGroupInfoCfg.find(uiTid);
		if (iter == m_mapMonsterGroupInfoCfg.end())
			return nullptr;
		return &(iter->second);
	}

	monster_param_t* MonsterConfig::GetMonsterParamConfig(const uint32 uiTid)
	{
		auto iter = m_mapMonsterParamCfg.find(uiTid);
		if (iter == m_mapMonsterParamCfg.end())
			return nullptr;
		return &(iter->second);
	}

	monster_info_t* MonsterConfig::GetMonsterInfoConfig(const uint32 uiTid)
	{
		auto iter = m_mapMonsterInfoCfg.find(uiTid);
		if (iter == m_mapMonsterInfoCfg.end())
			return nullptr;
		return &(iter->second);
	}
}
