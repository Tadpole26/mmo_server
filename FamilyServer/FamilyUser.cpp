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