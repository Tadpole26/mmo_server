#include "SkillConfig.h"
#include "util_strop.h"

namespace config
{
	bool SkillConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("active_skill", std::bind(&SkillConfig::AddActiveSkill, this));
		InitSheet("skill_effective", std::bind(&SkillConfig::AddActiveSkill, this));
		InitSheet("skill_effective_logic", std::bind(&SkillConfig::AddActiveSkill, this));
		InitSheet("skill_target", std::bind(&SkillConfig::AddActiveSkill, this));

		return LoadCfgFromStr(strContent);
	}

	bool SkillConfig::AddActiveSkill()
	{
		uint32 uiTid = GetDataUInt("id");
		active_skill_t& stCfg = m_mapActiveSkillCfg[uiTid];
		stCfg.uiTid = GetDataUInt("id");
		stCfg.uiMainId = GetDataUInt("main_id");
		stCfg.uiStage = GetDataUInt("stage");
		stCfg.uiLevel = GetDataUInt("level");
		stCfg.uiChargeStateFix = GetDataUInt("charge_state_fix");
		stCfg.uiChargeState = GetDataUInt("charge_state");
		stCfg.uiCastState = GetDataUInt("cast_state");
		stCfg.uiWaitState = GetDataUInt("wait_state");
		stCfg.uiColdTime = GetDataUInt("cold_time");
		GetDataItem("cost_value", stCfg.mapCost);
		stCfg.uiDistance = GetDataUInt("distance");
		stCfg.uiReleaseType = GetDataUInt("release_type");
		stCfg.uiEffectId = GetDataUInt("effect_id");
		stCfg.uiGroup = GetDataUInt("group");
		return true;
	}

	bool SkillConfig::AddSkillEffective()
	{
		uint32 uiTid = GetDataUInt("id");
		skill_effective_t& stCfg = m_mapSkillEftCfg[uiTid];
		stCfg.uiTid = GetDataUInt("id");
		str_split_num(GetDataChr("target"), '|', stCfg.vecTarget);
		stCfg.uiEffectiveId = GetDataUInt("effective_id");
		return true;
	}

	bool SkillConfig::AddSkillEffectiveLogic()
	{
		uint32 uiTid = GetDataUInt("id");
		skill_effective_logic_t& stCfg = m_mapSkillEftLogicCfg[uiTid];
		stCfg.uiTid = GetDataUInt("id");
		stCfg.uiDistanceType = GetDataUInt("distance_type");
		stCfg.uiAttType = GetDataUInt("attr_type");
		stCfg.uiDamageType = GetDataUInt("damage_type");
		stCfg.uiEffctive = GetDataUInt("effctive");
		stCfg.uiParma1 = GetDataUInt("parma1");
		stCfg.uiParma2 = GetDataUInt("parma2");
		stCfg.uiParma3 = GetDataUInt("parma3");
		stCfg.uiParma4 = GetDataUInt("parma4");
		stCfg.uiSkillType = GetDataUInt("skill_type");
		stCfg.bTriggerPassive = GetDataBool("trigger_passive");
		return true;
	}

	bool SkillConfig::AddSkillTarget()
	{
		uint32 uiTid = GetDataUInt("id");
		skill_target_t& stCfg = m_mapSkillTargetCfg[uiTid];
		stCfg.uiTid = GetDataUInt("id");
		stCfg.uiTargetRange = GetDataUInt("target_range");
		stCfg.uiShapeType = GetDataUInt("shape_type");
		str_split_num(GetDataChr("params"), '|', stCfg.vecParams);
		stCfg.uiGroup = GetDataUInt("group");
		return true;
	}
}

