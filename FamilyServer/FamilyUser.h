#pragma once
#include "sdk.h"
#include "cUser.h"

class FamilyUser : public cUser
{
public:
	FamilyUser(zRoleIdType roleId);
	~FamilyUser();
	bool init();
	void final();
public:
	uint32 getTickGroup() { return _tickGroup; }
public:
	void setTickGroup(uint32 val) { _tickGroup = val; }
private:
	uint32 _tickGroup = 0;
};
