#pragma once
#include "util_load_base.h"

struct active_skill_t
{
	uint32 uiTid = 0;						//tid 怪5位:技能主id*100+技能数量*10+段数 角色6位:技能主id*1000+技能数量*100+等级
	uint32 uiMainId = 0;						//技能主id
	uint32 uiStage = 0;						//怪:技能数量 角色:技能段数
	uint32 uiLevel = 0;						//怪:技能段数 角色:技能等级
	uint32 uiChargeStateFix = 0;				//可变吟唱状态
	uint32 uiChargeState = 0;				//固定吟唱状态
	uint32 uiCastState = 0;					//技能释放状态
	uint32 uiWaitState = 0;					//技能等待状态
	uint32 uiColdTime = 0;					//技能cd
	std::map<uint32, uint32> mapCost;			//技能消耗
	uint32 uiDistance = 1;					//施法距离(格子)
	uint32 uiReleaseType = 0;				//锁敌方式
	uint32 uiEffectId = 0;					//技能效果id
	uint32 uiGroup = 0;						//所属技能组

};

struct skill_effective_t
{
	uint32 uiTid = 0;
	std::vector<uint32> vecTarget;			//索敌id(对应技能索敌表)|条件id|条件id...
	uint32 uiEffectiveId = 0;				//对应技能效果逻辑表id
};

struct skill_effective_logic_t
{
	uint32 uiTid = 0;
	uint32 uiDistanceType = 0;				//1:近战 2:远程
	uint32 uiAttType = 0;					//属性类型:水、地、火、风、毒等等
	uint32 uiDamageType = 0;				//1:物理 2:魔法
	uint32 uiEffctive = 0;					//效果枚举:伤害、治疗、buff等等
	uint32 uiParma1 = 0;
	uint32 uiParma2 = 0;
	uint32 uiParma3 = 0;
	uint32 uiParma4 = 0;
	uint32 uiSkillType = 0;					//效果类型:主动、被动、buff等等
	bool bTriggerPassive = false;			//能否触发被动
};

struct skill_target_t
{
	uint32 uiTid = 0;
	uint32 uiTargetRange = 0;				//粗略索敌范围
	uint32 uiShapeType = 0;					//形状类型
	std::vector<uint32> vecParams;			//形状对应参数
	uint32 uiGroup = 0;						//阵营
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
