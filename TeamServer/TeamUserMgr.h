#pragma once

#include "sdk.h"
#include "singleton.h"

class TeamUser;
class TeamUserMgr :public zRoleMgr<TeamUser>
{
public:
	TeamUserMgr();
	~TeamUserMgr();
public:
	TeamUser* tryCreateRole(uint32 zoneIdF, zRoleIdType roleId);
};

#define gTeamUserMgr Singleton<TeamUserMgr>::getInstance()
