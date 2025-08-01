#pragma once
#include "sdk.h"

class GateUser;
class GateUserMgr
{
public:
	GateUserMgr();
	~GateUserMgr();
	bool init();
	void final();
	void tick(uint32 tickSec);
public:
	GateUser* createRole(zRoleIdType roleId);
	GateUser* getRole(zRoleIdType roleId);
	void destroyRole(GateUser* role);
private:
	std::unordered_map<zRoleIdType, GateUser*> _alluser;
};