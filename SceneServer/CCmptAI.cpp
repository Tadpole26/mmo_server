#include "zType.h"
#include "log_mgr.h"

#include "CCmptAI.h"
#include "CCmptView.h"
CCmptAI::CCmptAI()
{
}

CCmptAI::~CCmptAI()
{
}

std::vector<CCmptAI::StateHandle> CCmptAI::kStateHandles
{
	&CCmptAI::StateIdle,
	&CCmptAI::StateSelectSkill,
	&CCmptAI::StateCastSkill,
	&CCmptAI::StateChase,
	&CCmptAI::StateChaseWait,
	&CCmptAI::StateFollow,
	&CCmptAI::StateBackHome,
	&CCmptAI::StateBackHomeWait,
	&CCmptAI::StatePatrol,									//戒备
	&CCmptAI::StatePatrolWait,
	&CCmptAI::StateSelfKill,
	&CCmptAI::StateAssist,
	&CCmptAI::StateRest,
	&CCmptAI::StateForceMove,
};

bool CCmptAI::CanRest()
{
	//if (九屏内有玩家) return false;
	return true;
}

bool CCmptAI::CanActiveAttack()
{
	return GetActiveAttachRange();
}

uint32 CCmptAI::GetCastSkillRange(uint32 uiSkillId)
{
	if (!uiSkillId)
	{
		assert(false);
		return 0;
	}
	return 1;
}

void CCmptAI::SwitchState(uint32 uiState)
{
	uint32 uiOld = m_uiState;
	m_uiState = uiState;
	Log_Custom("ai", "oldstate:%u to state:%u", uiOld, m_uiState);
}

bool CCmptAI::StateLoop()
{
	auto pHandle = kStateHandles[m_uiState];
	assert(pHandle);
	Log_Custom("ai", "state loop state:%u", m_uiState);
	return (this->*pHandle)();
}

bool CCmptAI::StateIdle()
{
	if (CanRest())
	{
		if (GetRestExpire() == 0)
		{
			Log_Custom("ai", "start waiting for rest");
			SetRestExpire((uint64)GetMillSec() + 3000);
		}
		else
		{
			uint64 ullCurTime = (uint64)GetMillSec();
			if (GetRestExpire() <= ullCurTime)
			{
				Log_Custom("ai", "start rest");
				SwitchState(AIState_Rest);
				return true;
			}
			Log_Custom("ai", "waiting for rest");
		}
	}
	else
	{
		if (GetRestExpire() > 0)
		{
			Log_Custom("ai", "stop rest");
			SetRestExpire(0);
		}
	}
	CCmptView* pTarget = GetFightTarget();
	if (pTarget)
	{
		Log_Custom("ai", "new target:%u, %ull", pTarget->GetViewType(), pTarget->GetViewId());
		SwitchState(AIState_ChaseWait);
		return true;
	}
	if (CanActiveAttack())
	{
		Log_Custom("ai", "start scan");
		pTarget = ScanFightTarget();
		if (pTarget)
		{
			Log_Custom("ai", "target found:%u, %ull", pTarget->GetViewType(), pTarget->GetViewId());
			SwitchState(AIState_ChaseWait);
			return true;
		}
		Log_Custom("ai", "target not found");
	}
	if (CanPatrol())
	{
		Log_Custom("ai", "start patrol");
		SetPatrolExpire((uint64)GetMillSec() + PatrolInterval());
		SwitchState(AIState_PatrolWait);
		return true;
	}
	Log_Custom("ai", "none");
	return true;
}

bool CCmptAI::StateSelectSkill()
{
	CCmptView* pTarget = GetFightTarget();
	if (!pTarget)
	{
		Log_Custom("ai", "lost target");
		SwitchState(AIState_BackHomeWait);
		return true;
	}
	if (!CheckCastSkillCD())
	{
		Log_Custom("ai", "cast waiting");
		return true;
	}
	uint32 uiSkillId = GetNextSkill();
	if (!uiSkillId)
	{
		Log_Custom("ai", "try select skill");
		uiSkillId = SelectSkill();
		if (!m_oForceDst.empty())
		{
			Log_Custom("ai", "into moving");
			SwitchState(AIState_ForceMove);
			return true;
		}
		if (!uiSkillId)
		{
			Log_Custom("ai", "no skill selected");
			SwitchState(AIState_Chase);
			return true;
		}
		Log_Custom("ai", "select new skill: %d", uiSkillId);
	}
	CCmptView* pSkillTarget = GetNextSkillTarget();
	CCmptView* pChaseTarget = pSkillTarget ? pSkillTarget : pTarget;
	uint32 uiDistance = GetCastSkillRange(uiSkillId);
	//距离是否在范围
	Log_Custom("ai", "ready to cast:%u, %u", uiSkillId, uiDistance);
	SwitchState(AIState_CastSkill);
	return true;
}

bool CCmptAI::StateCastSkill()
{
	uint32 uiSkillId = GetNextSkill();
	if (!uiSkillId)
	{
		Log_Custom("ai", "lose skill");
		SwitchState(AIState_SelectSkill);
		return true;
	}
	//如果在移动,停止移动
	CastSkill();
	ClearSkill();
	Log_Custom("ai", "cast skill %u", uiSkillId);
	SwitchState(AIState_SelectSkill);
	return true;
}

bool CCmptAI::StateChase()
{
	if (FarAwayFromHome(CmptViewPtr()->GetPos()))
	{
		Log_Custom("ai", "far away from home");
		SwitchState(AIState_BackHomeWait);
		return true;
	}
	auto* pTarget = GetFightTarget();
	if (!pTarget)
	{
		Log_Custom("ai", "lose target");
		SwitchState(AIState_BackHomeWait);
		return true;
	}
	if (FarAwayFromHome(pTarget->GetPos()))
	{
		Log_Custom("ai", "far away from target:%u, %ull", pTarget->GetViewType(), pTarget->GetViewId());
		RemoveFightTarget(*pTarget);
		pTarget = GetFightTarget();
		if (!pTarget)
			SwitchState(AIState_BackHomeWait);
		return true;
	}
	uint32 uiSkillId = GetNextSkill();
	if (!uiSkillId)
	{
		Log_Custom("ai", "lose skill");
		SwitchState(AIState_SelectSkill);
		return true;
	}
	CCmptView* pSkillTarget = GetNextSkillTarget();
	CCmptView* pChaseTarget = pSkillTarget ? pSkillTarget : pTarget;
	const zPos& oTargetPos = pChaseTarget->GetPos();
	uint32 uiDistance = GetCastSkillRange(uiSkillId);
	//在技能释放范围,改变技能释放状态
	return true;
}

bool CCmptAI::StateChaseWait()
{
	Log_Custom("ai", "chase begin");
	SwitchState(AIState_Chase);
	return true;
}

bool CCmptAI::StateFollow()
{
	return true;
}

bool CCmptAI::StateBackHome()
{
	if (CmptViewRef().IsMovingWalking())
	{
		Log_Custom("ai", "moving");
		return true;
	}
	Log_Custom("ai", "not moving");
	auto oPos = GetHomePos();
	if (oPos.empty())
	{
		Log_Custom("ai", "lose home pos");
		SwitchState(AIState_Idle);
		return true;
	}
	if (oPos.getDistance(CmptViewRef().GetPos()) < 2)
	{
		Log_Custom("ai", "reach home");
		if (HomeWaiting())
		{
			Log_Custom("ai", "home waiting");
			return true;
		}
		SwitchState(AIState_Idle);
		return true;
	}
	Log_Custom("ai", "move(%u,%u)", oPos.x, oPos.y);
	CmptViewRef().MoveTo(oPos);
	return true;
}

bool CCmptAI::StateBackHomeWait()
{
	ClearAllFightTarget();
	if (CmptViewRef().IsMovingWalking())
	{
		//CmptViewRef().MoveTo();
	}
	SwitchState(AIState_BackHome);
	Log_Custom("ai", "start back home");
	return true;
}

bool CCmptAI::StatePatrol()
{
	//离家过远,回家
	if (FarAwayFromHome(CmptViewRef().GetPos()))
	{
		Log_Custom("ai", "far away from home");
		SwitchState(AIState_BackHomeWait);
		return true;
	}
	CCmptView* pTarget = GetFightTarget();
	if (pTarget)
	{
		Log_Custom("ai", "target appear:%u, %llu", pTarget->GetViewType(), pTarget->GetViewId());
		SwitchState(AIState_ChaseWait);
		return true;
	}
	if (CanActiveAttack())
	{
		Log_Custom("ai", "start scan");
		pTarget = ScanFightTarget();
		if (pTarget)
		{
			Log_Custom("ai", "target found:%u, %llu", pTarget->GetViewType(), pTarget->GetViewId());
			SwitchState(AIState_ChaseWait);
			return true;
		}
		Log_Custom("ai", "target not found");
	}
	//找不到目标
	if (!CmptViewRef().IsMovingWalking())
	{
		Log_Custom("ai", "not moving");
		SwitchState(AIState_Idle);
		return true;
	}
	Log_Custom("ai", "moving");
	return true;
}

bool CCmptAI::StatePatrolWait()
{
	CCmptView* pTarget = GetFightTarget();
	if (pTarget)
	{
		Log_Custom("ai", "new target:%u, %llu", pTarget->GetViewType(), pTarget->GetViewId());
		SwitchState(AIState_ChaseWait);
		return true;
	}
	if (CanActiveAttack())
	{
		Log_Custom("ai", "start scan");
		pTarget = ScanFightTarget();
		if (pTarget)
		{
			Log_Custom("ai", "target found:%u, %llu", pTarget->GetViewType(), pTarget->GetViewId());
			SwitchState(AIState_ChaseWait);
			return true;
		}
		Log_Custom("ai", "target not found");
	}
	Log_Custom("ai", "patrol start");
	zPos oPos = RandomPatrolPos();
	if (oPos.empty())
	{
		Log_Custom("ai", "lose patrol pos");
		SwitchState(AIState_Idle);
		return true;
	}
	Log_Custom("ai", "move(%u, %u)", oPos.x, oPos.y);
	CmptViewRef().MoveTo(oPos);
	SwitchState(AIState_Patrol);
	return true;
}

bool CCmptAI::StateSelfKill()
{
	return true;
}

bool CCmptAI::StateAssist()
{
	return true;
}

bool CCmptAI::StateRest()
{
	return true;
}

bool CCmptAI::StateForceMove()
{
	return true;
}

