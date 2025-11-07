#include "FamilyUser.h"
#include "FamilyUserMgr.h"


FamilyUser* FamilyUserMgr::getUser(zRoleIdType roleId)
{
	auto it = _users.find(roleId);
	if (it == _users.end()) return nullptr;
	return it->second;
}

FamilyUser* FamilyUserMgr::tryCreateUser(zRoleIdType roleId)
{
	auto* pUser = getUser(roleId);
	if (pUser) return pUser;
	pUser = new FamilyUser(roleId);
	_tickGroup = (_tickGroup + 1) % cFamily::tickGroupSize;
	pUser->setTickGroup(_tickGroup);
	if (!_users.emplace(roleId, pUser).second)
	{
		Log_Error("tryCreateUser.!_users.emplace(roleId, pUser).second.%lu", roleId);
		pUser->final();
		SAFE_DELETE(pUser);
		return nullptr;
	}
	Log_Info("tryCreateUser.%u",roleId);
	return pUser;
}

void FamilyUserMgr::destroyUser(FamilyUser*& pUser)
{
	_users.erase(pUser->getRoleId());
	pUser->final();
	SAFE_DELETE(pUser);
}

void FamilyUserMgr::__tickUser(uint32 group)
{
	assert(group < cFamily::tickGroupSize);
	auto& lst = _groupIndex[group];
	std::for_each(lst.begin(), lst.end(), [](FamilyUser* pUser) { pUser->tick(); });
}