#pragma once
#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct monster_group_info_t
	{
		uint32 uiTid = 0;
		uint32 uiMonsterGroupType = 0;						//怪物组类型
		bool bAssist = false;								//受否协同组
		uint32 uiCreateTime = 0;							//出生时机
		uint32 uiMaxGroupNum = 0;							//总怪物组上限
		uint32 uiRebornType = 0;							//怪物重生类型
		uint32 uiRebornNum = 0;								//重生次数
		uint32 uiRebornCd = 0;								//重生时间
		uint32 uiTransType = 0;								//位置类型
		std::vector<uint32> vecMonsterIds;					//怪物参数表
		uint32 uiRandNum = 0;								//随机怪物组数量
	};

	struct monster_param_t
	{
		uint32 uiTid = 0;
		uint32 uiMonsterId = 0;								//怪物id				
		uint32 uiNumber = 0;								//该怪物id生成的数量	
		int	iLocationOffsetX = 0;							//相对笔刷位置X
		int iLocationOffsetY = 0;							//相对笔刷位置Y
		int	iRotationOffset = 0;							//相对朝向
	};

	struct monster_info_t
	{
		uint32 uiTid = 0;
		uint32 uiLevel = 0;									//等级
		uint32 uiMonsterRank = 0;							//怪物阶级
		uint32 uiAttrId = 0;								//怪物属性id
		uint32 uiDieVanish = 0;								//死亡消失时间
		std::string strAiName;								//AI文件名
		uint32 uiInterval = 0;								//索敌间隔
		uint32 uiSkillId = 0;								//主动技能
		std::vector<uint32> vecViewRange;					//视野范围
		std::vector<uint32> vecAlertRange;					//警戒范围
		std::map<uint32, uint32> mapPatrolRadius;			//巡逻格
		std::vector<uint32> vecPatrolInterval;				//巡逻间隔
		uint32 uiPatrolSpeed = 0;							//巡逻移速
		uint32 uiChaseSpeed = 0;							//追逐移速
		uint32 uiDropId = 0;								//掉落表id
		uint32 uiPhysique = 0;								//怪物体型
		uint32 uiRace = 0;									//怪物种族
		uint32 uiNature = 0;								//属性
		uint32 uiNatureLv = 0;								//属性等级
		uint32 uiAreaRadius = 0;							//占地面积
		uint32 uiActionTime = 0;							//出生表演时间
		uint32 uiFallTime = 0;								//倒下时间
		uint32 uiLeaveTime = 0;								//离开时间
		uint32 uiHatchTime = 0;								//孵化时间
		uint32 uiHatchTid = 0;								//孵化对象
		std::vector<uint32> vecPetCapture;					//宠物抓捕
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
