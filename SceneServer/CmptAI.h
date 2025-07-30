#pragma once

#include "sdk.h"
class CCmptView;
//AI组件基类
class CmptAI
{
public:
	using StateHandle = bool (CmptAI::*)();
public:
	enum AIState
	{
		AIState_Idle,
		AIState_SelectSkill,
		AIState_CastSkill,
		AIState_Chase,
		AIState_ChaseWait,
		AIState_Follow,
		AIState_BackHome,
		AIState_BackHomeWait,
		AIState_Patrol,
		AIState_PatrolWait,
		AIState_SelfKill,
		AIState_Assist,
		AIState_Rest,
		AIState_ForceMove,

		AIState_Max
	};
private:
	static std::vector<StateHandle> kStateHandles;
private:
	//static const char* stateToStr(uint32 uiState);
public:
	CmptAI();
	virtual ~CmptAI();
	virtual bool init() = 0;
	virtual bool loop() = 0;
	virtual CCmptView* cmptViewPtr() = 0;
	virtual CCmptView& cmptViewRef() = 0;
public:
	//仇恨
	virtual CCmptView* getFightTarget() = 0;
	virtual CCmptView* getRandomFightTarget() = 0;
	virtual CCmptView* scanFightTarget() = 0;
	virtual void setFightTarget(CCmptView &oTarget) = 0;
	virtual void removeFightTarget(CCmptView &oTarget) = 0;
	virtual void clearAllFightTarget() = 0;
	virtual bool isHatredOpen() = 0;
	virtual bool hasHatred() { return false; }
	virtual void addHatred(CCmptView& oTarget, uint32 uiValue) {}
	virtual void subHatred(CCmptView& oTarget, uint32 uiValue) {}
	virtual void addHatredPer(CCmptView& oTarget, uint32 uiPer) {}
	virtual void subHatredPer(CCmptView& oTarget, uint32 uiPer) {}
	virtual void addHatedBy(CCmptView& oFrom) {}
	virtual void removeHatedBy(CCmptView& oFrom) {}
	virtual bool foreachHatedBy(std::function<bool(CCmptView&)> func) { return true; }
	//巡逻
	virtual zPos randomPatrolPos() = 0;
	virtual zPos getHomePos() { return {}; }
	virtual uint32 getHomeRange() { return MAX_SCREEN_GRID; }
	virtual void onBackHome() {}
	virtual void onReachHome() {}
	virtual bool homeWaiting() { return true; }
	virtual bool canPatrol() = 0;
	bool farAwayFromHome(const zPos& oPos) { return true; }
	virtual uint32 patrolInterval() { return 1000; }
	virtual uint64 getPatrolExpire() { return 0; }
	virtual void setPatrolExpire(uint64 ullExpire) {}
	virtual void onEnterChase() {}
	//void MoveToPos(const zPos& oPos);
	//技能
	virtual void setNextSkillTarget(uint32 uiType, uint64 ullUid) = 0;
	virtual void setNextSkillPos(const zPos& oPos) = 0;
	virtual CCmptView* getNextSkillTarget() = 0;
	virtual uint32 getNextSkill() = 0;
	virtual void setNextSkill(uint32 uiSkillId) = 0;
	virtual uint32 selectSkill() = 0;
	virtual void castSkill() = 0;
	virtual void clearSkill() = 0;
	virtual uint32 getActiveAttachRange() = 0;
	virtual bool checkCastSkillCD() = 0;
	virtual uint32 getSkillByOrder(uint32 uiOrder) { return 0; }
	bool canActiveAttack();
	uint32 getCastSkillRange(uint32 uiSkillId);
	//uint32 GetAttachRange();
	//无状态(rest)
	virtual bool canRest();
	//说话
	virtual void speak(uint32 uiType, uint32 uiTid) {}
	virtual void addCountDown(uint32 uiId, uint32 uiLasting, uint32 uiDelay) {}
	virtual uint32 getCountDown(uint32 uiId) { return 0; }
public:
	void switchState(uint32 uiState);
	bool stateLoop();
	uint32 getState() { return _state; }
	bool stateIdle();											//空闲状态
	bool stateSelectSkill();
	bool stateCastSkill();
	bool stateChase();
	bool stateChaseWait();
	bool stateFollow();
	bool stateBackHome();
	bool stateBackHomeWait();
	bool statePatrol();
	bool statePatrolWait();
	bool stateSelfKill();
	bool stateAssist();
	bool stateRest();
	bool stateForceMove();

public:
	uint64 getRestExpire() { return _restExpire; }
	void setRestExpire(uint64 expire) { _restExpire = expire; }
private:
	uint32 _state = AIState_Rest;
	uint64 _restExpire = 0;
	zPos _forceMoveDst;
};