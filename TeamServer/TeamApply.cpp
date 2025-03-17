#include "teamcommon.pb.h"

#include "TeamApply.h"
#include "ParamConfig.h"
#include "TeamUser.h"

TeamApply::TeamApply(TeamUser* pUser, zSceneIdType leaderSceneId)
{
	this->pUser = pUser;
	this->expire = GetCurrTime() + gParamCfg->teamApplyExpireSec;
	this->leaderSceneId = leaderSceneId;
}

TeamApply::~TeamApply()
{
}

void TeamApply::final()
{
}

bool TeamApply::isExpire()
{
	return this->expire <= GetCurrTime();
}

void TeamApply::fill(common::TeamApply& out)
{
	pUser->fill(*out.mutable_role());
	out.set_expire(expire);
	out.set_leadersceneid(leaderSceneId);
}