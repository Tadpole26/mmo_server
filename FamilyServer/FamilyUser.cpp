#include "FamilyUser.h"

FamilyUser::FamilyUser(zRoleIdType roleId) : cUser(roleId)
{
}

FamilyUser::~FamilyUser()
{
}

bool FamilyUser::init()
{
	return true;
}

void FamilyUser::final()
{
}

void FamilyUser::evGameLogin(const inner::InnerGameEvent& in)
{
	Log_Info("evGameLogin");
}

void FamilyUser::evGameLogout(const inner::InnerGameEvent& in)
{
	Log_Info("evGameLogout");
}

void FamilyUser::evGameOnline(const inner::InnerGameEvent& in)
{
	Log_Info("evGameOnline");
}

void FamilyUser::evGameOffline(const inner::InnerGameEvent& in)
{
	Log_Info("evGameOffline");
}

void FamilyUser::evGameChangeMap(const inner::InnerGameEvent& in)
{
	Log_Info("evGameChangeMap");
}

void FamilyUser::evGameWakeup(const inner::InnerGameEvent& in)
{
	Log_Info("evGameWakeup");
}
