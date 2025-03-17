#include "innercommon.pb.h"

#include "TeamUser.h"


void TeamUser::evGameDestroy(const inner::InnerGameEvent& in)
{
}

void TeamUser::evGameLogin(const inner::InnerGameEvent& in)
{
	assert(in.login().has_teamsvr());
	setBrief(in.login().teamsvr().brief());
	setExtd1(in.login().teamsvr().extd1());
	setExtd2(in.login().teamsvr().extd2());
	setExtd3(in.login().teamsvr().extd3());
	setF33(in.login().teamsvr().f33());
	setF55(in.login().teamsvr().f55());
	setSceneHashId(in.login().teamsvr().scenehashid());
}

void TeamUser::evGameLogout(const inner::InnerGameEvent& in)
{
}

void TeamUser::evGameOnline(const inner::InnerGameEvent& in)
{
}

void TeamUser::evGameOffline(const inner::InnerGameEvent& in)
{
}

void TeamUser::evGameDeath(const inner::InnerGameEvent& in)
{
}

void TeamUser::evGameRebirth(const inner::InnerGameEvent& in)
{
}

void TeamUser::evGameWakeup(const inner::InnerGameEvent& in)
{
}

void TeamUser::evGameChangeMap(const inner::InnerGameEvent& in)
{
}