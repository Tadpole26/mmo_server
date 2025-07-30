#include "CmptAI.h"
#include "CCmptView.h"
CmptAI::CmptAI()
{
}

CmptAI::~CmptAI()
{
}

std::vector<CmptAI::StateHandle> CmptAI::kStateHandles
{
	&CmptAI::stateIdle,
	&CmptAI::stateSelectSkill,
	&CmptAI::stateCastSkill,
	&CmptAI::stateChase,
	&CmptAI::stateChaseWait,
	&CmptAI::stateFollow,
	&CmptAI::stateBackHome,
	&CmptAI::stateBackHomeWait,
	&CmptAI::statePatrol,									//戒备
	&CmptAI::statePatrolWait,
	&CmptAI::stateSelfKill,
	&CmptAI::stateAssist,
	&CmptAI::stateRest,
	&CmptAI::stateForceMove,
};

bool CmptAI::canRest()
{
	//if (cmptViewRef().sizeNine)
	//if (九屏内有玩家) return false;
	return true;
}

bool CmptAI::canActiveAttack()
{
	return getActiveAttachRange();
}

uint32 CmptAI::getCastSkillRange(uint32 uiSkillId)
{
	if (!uiSkillId)
	{
		assert(false);
		return 0;
	}
	return 1;
}

void CmptAI::switchState(uint32 uiState)
{
	uint32 uiOld = _state;
	_state = uiState;
	Log_Custom("ai", "oldstate:%u to state:%u", uiOld, _state);
}

bool CmptAI::stateLoop()
{
	auto pHandle = kStateHandles[_state];
	assert(pHandle);
	Log_Custom("ai", "state loop state:%u", _state);
	return (this->*pHandle)();
}

bool CmptAI::stateIdle()
{
	if (canRest())
	{
		if (getRestExpire() == 0)
		{
			Log_Custom("ai", "start waiting for rest");
			setRestExpire((uint64)GetMillSec() + 3000);
		}
		else
		{
			uint64 ullCurTime = (uint64)GetMillSec();
			if (getRestExpire() <= ullCurTime)
			{
				Log_Custom("ai", "start rest");
				switchState(AIState_Rest);
				return true;
			}
			Log_Custom("ai", "waiting for rest");
		}
	}
	else
	{
		if (getRestExpire() > 0)
		{
			Log_Custom("ai", "stop rest");
			setRestExpire(0);
		}
	}
	CCmptView* pTarget = getFightTarget();
	if (pTarget)
	{
		Log_Custom("ai", "new target:%u, %ull", pTarget->GetViewType(), pTarget->GetViewId());
		switchState(AIState_ChaseWait);
		return true;
	}
	if (canActiveAttack())
	{
		Log_Custom("ai", "start scan");
		pTarget = scanFightTarget();
		if (pTarget)
		{
			Log_Custom("ai", "target found:%u, %ull", pTarget->GetViewType(), pTarget->GetViewId());
			switchState(AIState_ChaseWait);
			return true;
		}
		Log_Custom("ai", "target not found");
	}
	if (canPatrol())
	{
		Log_Custom("ai", "start patrol");
		setPatrolExpire((uint64)GetMillSec() + patrolInterval());
		switchState(AIState_PatrolWait);
		return true;
	}
	Log_Custom("ai", "none");
	return true;
}

bool CmptAI::stateSelectSkill()
{
	CCmptView* pTarget = getFightTarget();
	if (!pTarget)
	{
		Log_Custom("ai", "lost target");
		switchState(AIState_BackHomeWait);
		return true;
	}
	if (!checkCastSkillCD())
	{
		Log_Custom("ai", "cast waiting");
		return true;
	}
	uint32 uiSkillId = getNextSkill();
	if (!uiSkillId)
	{
		Log_Custom("ai", "try select skill");
		uiSkillId = selectSkill();
		if (!_forceMoveDst.empty())
		{
			Log_Custom("ai", "into moving");
			switchState(AIState_ForceMove);
			return true;
		}
		if (!uiSkillId)
		{
			Log_Custom("ai", "no skill selected");
			switchState(AIState_Chase);
			return true;
		}
		Log_Custom("ai", "select new skill: %d", uiSkillId);
	}
	CCmptView* pSkillTarget = getNextSkillTarget();
	CCmptView* pChaseTarget = pSkillTarget ? pSkillTarget : pTarget;
	uint32 uiDistance = getCastSkillRange(uiSkillId);
	//距离是否在范围
	Log_Custom("ai", "ready to cast:%u, %u", uiSkillId, uiDistance);
	switchState(AIState_CastSkill);
	return true;
}

bool CmptAI::stateCastSkill()
{
	uint32 uiSkillId = getNextSkill();
	if (!uiSkillId)
	{
		Log_Custom("ai", "lose skill");
		switchState(AIState_SelectSkill);
		return true;
	}
	//如果在移动,停止移动
	castSkill();
	clearSkill();
	Log_Custom("ai", "cast skill %u", uiSkillId);
	switchState(AIState_SelectSkill);
	return true;
}

bool CmptAI::stateChase()
{
	if (farAwayFromHome(cmptViewPtr()->GetPos()))
	{
		Log_Custom("ai", "far away from home");
		switchState(AIState_BackHomeWait);
		return true;
	}
	auto* pTarget = getFightTarget();
	if (!pTarget)
	{
		Log_Custom("ai", "lose target");
		switchState(AIState_BackHomeWait);
		return true;
	}
	if (farAwayFromHome(pTarget->GetPos()))
	{
		Log_Custom("ai", "far away from target:%u, %ull", pTarget->GetViewType(), pTarget->GetViewId());
		removeFightTarget(*pTarget);
		pTarget = getFightTarget();
		if (!pTarget)
			switchState(AIState_BackHomeWait);
		return true;
	}
	uint32 uiSkillId = getNextSkill();
	if (!uiSkillId)
	{
		Log_Custom("ai", "lose skill");
		switchState(AIState_SelectSkill);
		return true;
	}
	CCmptView* pSkillTarget = getNextSkillTarget();
	CCmptView* pChaseTarget = pSkillTarget ? pSkillTarget : pTarget;
	const zPos& oTargetPos = pChaseTarget->GetPos();
	uint32 uiDistance = getCastSkillRange(uiSkillId);
	//在技能释放范围,改变技能释放状态
	return true;
}

bool CmptAI::stateChaseWait()
{
	Log_Custom("ai", "chase begin");
	switchState(AIState_Chase);
	return true;
}

bool CmptAI::stateFollow()
{
	return true;
}

bool CmptAI::stateBackHome()
{
	if (cmptViewRef().IsMovingWalking())
	{
		Log_Custom("ai", "moving");
		return true;
	}
	Log_Custom("ai", "not moving");
	auto oPos = getHomePos();
	if (oPos.empty())
	{
		Log_Custom("ai", "lose home pos");
		switchState(AIState_Idle);
		return true;
	}
	if (oPos.getDistance(cmptViewRef().GetPos()) < 2)
	{
		Log_Custom("ai", "reach home");
		if (homeWaiting())
		{
			Log_Custom("ai", "home waiting");
			return true;
		}
		switchState(AIState_Idle);
		return true;
	}
	Log_Custom("ai", "move(%u,%u)", oPos.x, oPos.y);
	cmptViewRef().MoveTo(oPos);
	return true;
}

bool CmptAI::stateBackHomeWait()
{
	clearAllFightTarget();
	if (cmptViewRef().IsMovingWalking())
	{
		//CmptViewRef().MoveTo();
	}
	switchState(AIState_BackHome);
	Log_Custom("ai", "start back home");
	return true;
}

bool CmptAI::statePatrol()
{
	//离家过远,回家
	if (farAwayFromHome(cmptViewRef().GetPos()))
	{
		Log_Custom("ai", "far away from home");
		switchState(AIState_BackHomeWait);
		return true;
	}
	CCmptView* pTarget = getFightTarget();
	if (pTarget)
	{
		Log_Custom("ai", "target appear:%u, %llu", pTarget->GetViewType(), pTarget->GetViewId());
		switchState(AIState_ChaseWait);
		return true;
	}
	if (canActiveAttack())
	{
		Log_Custom("ai", "start scan");
		pTarget = scanFightTarget();
		if (pTarget)
		{
			Log_Custom("ai", "target found:%u, %llu", pTarget->GetViewType(), pTarget->GetViewId());
			switchState(AIState_ChaseWait);
			return true;
		}
		Log_Custom("ai", "target not found");
	}
	//找不到目标
	if (!cmptViewRef().IsMovingWalking())
	{
		Log_Custom("ai", "not moving");
		switchState(AIState_Idle);
		return true;
	}
	Log_Custom("ai", "moving");
	return true;
}

bool CmptAI::statePatrolWait()
{
	CCmptView* pTarget = getFightTarget();
	if (pTarget)
	{
		Log_Custom("ai", "new target:%u, %llu", pTarget->GetViewType(), pTarget->GetViewId());
		switchState(AIState_ChaseWait);
		return true;
	}
	if (canActiveAttack())
	{
		Log_Custom("ai", "start scan");
		pTarget = scanFightTarget();
		if (pTarget)
		{
			Log_Custom("ai", "target found:%u, %llu", pTarget->GetViewType(), pTarget->GetViewId());
			switchState(AIState_ChaseWait);
			return true;
		}
		Log_Custom("ai", "target not found");
	}
	Log_Custom("ai", "patrol start");
	zPos oPos = randomPatrolPos();
	if (oPos.empty())
	{
		Log_Custom("ai", "lose patrol pos");
		switchState(AIState_Idle);
		return true;
	}
	Log_Custom("ai", "move(%u, %u)", oPos.x, oPos.y);
	cmptViewRef().MoveTo(oPos);
	switchState(AIState_Patrol);
	return true;
}

bool CmptAI::stateSelfKill()
{
	return true;
}

bool CmptAI::stateAssist()
{
	return true;
}

bool CmptAI::stateRest()
{
	return true;
}

bool CmptAI::stateForceMove()
{
	return true;
}

