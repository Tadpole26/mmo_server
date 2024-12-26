#pragma once

#include "sdk.h"
class CCmptView;
//AI组件基类
class CCmptAI
{
public:
	using StateHandle = bool (CCmptAI::*)();
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
	CCmptAI();
	virtual ~CCmptAI();
	virtual bool Init() = 0;
	virtual bool Loop() = 0;
	virtual CCmptView* CmptViewPtr() = 0;
	virtual CCmptView& CmptViewRef() = 0;
public:
	//仇恨
	virtual CCmptView* GetFightTarget() = 0;
	virtual CCmptView* GetRandomFightTarget() = 0;
	virtual CCmptView* ScanFightTarget() = 0;
	virtual void SetFightTarget(CCmptView &oTarget) = 0;
	virtual void RemoveFightTarget(CCmptView &oTarget) = 0;
	virtual void ClearAllFightTarget() = 0;
	virtual bool IsHatredOpen() = 0;
	virtual bool HasHatred() { return false; }
	virtual void AddHatred(CCmptView& oTarget, uint32 uiValue) {}
	virtual void SubHatred(CCmptView& oTarget, uint32 uiValue) {}
	virtual void AddHatredPer(CCmptView& oTarget, uint32 uiPer) {}
	virtual void SubHatredPer(CCmptView& oTarget, uint32 uiPer) {}
	virtual void AddHatedBy(CCmptView& oFrom) {}
	virtual void RemoveHatedBy(CCmptView& oFrom) {}
	virtual bool foreachHatedBy(std::function<bool(CCmptView&)> func) { return true; }
	//巡逻
	virtual zPos RandomPatrolPos() = 0;
	virtual zPos GetHomePos() { return {}; }
	virtual uint32 GetHomeRange() { return MAX_SCREEN_GRID; }
	virtual void OnBackHome() {}
	virtual void OnReachHome() {}
	virtual bool HomeWaiting() { return true; }
	virtual bool CanPatrol() = 0;
	bool FarAwayFromHome(const zPos& oPos) { return true; }
	virtual uint32 PatrolInterval() { return 1000; }
	virtual uint64 GetPatrolExpire() { return 0; }
	virtual void SetPatrolExpire(uint64 ullExpire) {}
	virtual void OnEnterChase() {}
	//void MoveToPos(const zPos& oPos);
	//技能
	virtual void SetNextSkillTarget(uint32 uiType, uint64 ullUid) = 0;
	virtual void SetNextSkillPos(const zPos& oPos) = 0;
	virtual CCmptView* GetNextSkillTarget() = 0;
	virtual uint32 GetNextSkill() = 0;
	virtual void SetNextSkill(uint32 uiSkillId) = 0;
	virtual uint32 SelectSkill() = 0;
	virtual void CastSkill() = 0;
	virtual void ClearSkill() = 0;
	virtual uint32 GetActiveAttachRange() = 0;
	virtual bool CheckCastSkillCD() = 0;
	virtual uint32 GetSkillByOrder(uint32 uiOrder) { return 0; }
	bool CanActiveAttack();
	uint32 GetCastSkillRange(uint32 uiSkillId);
	//uint32 GetAttachRange();
	//无状态(rest)
	virtual bool CanRest();
	//说话
	virtual void Speak(uint32 uiType, uint32 uiTid) {}
	virtual void AddCountDown(uint32 uiId, uint32 uiLasting, uint32 uiDelay) {}
	virtual uint32 GetCountDown(uint32 uiId) { return 0; }
public:
	void SwitchState(uint32 uiState);
	bool StateLoop();
	uint32 GetState() { return m_uiState; }
	bool StateIdle();											//空闲状态
	bool StateSelectSkill();
	bool StateCastSkill();
	bool StateChase();
	bool StateChaseWait();
	bool StateFollow();
	bool StateBackHome();
	bool StateBackHomeWait();
	bool StatePatrol();
	bool StatePatrolWait();
	bool StateSelfKill();
	bool StateAssist();
	bool StateRest();
	bool StateForceMove();

public:
	uint64 GetRestExpire() { return m_ullRestExpire; }
	void SetRestExpire(uint64 ullExpire) { m_ullRestExpire = ullExpire; }
private:
	uint32 m_uiState = AIState_Rest;
	uint64 m_ullRestExpire = 0;
	zPos m_oForceDst;
};