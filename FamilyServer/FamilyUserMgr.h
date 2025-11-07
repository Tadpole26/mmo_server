#pragma once
#include "sdk.h"
#include "cFamily.h"

class FamilyUser;
class FamilyUserMgr
{
public:
	FamilyUserMgr();
	~FamilyUserMgr();
	bool init();
public:
	FamilyUser* getUser(zRoleIdType roleId);
	FamilyUser* tryCreateUser(zRoleIdType roleId);
	void destroyUser(FamilyUser *& pUser);
public:
	void __tickUser(uint32 group);
private:
	std::unordered_map<zRoleIdType, FamilyUser*> _users;
	std::array<std::list<FamilyUser*>, cFamily::tickGroupSize> _groupIndex;
	uint32 _tickGroup = 0;
};

#define gFamilyUserMgr Singleton<FamilyUserMgr>::getInstance()
