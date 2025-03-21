#include "TeamUserMgr.h"
#include "TeamUser.h"

TeamUserMgr::TeamUserMgr()
{
}

TeamUserMgr::~TeamUserMgr()
{
}

TeamUser* TeamUserMgr::tryCreateRole(uint32 zoneIdF, zRoleIdType roleId)
{
	auto* pUser = getRole(roleId);
	if (pUser != nullptr)
		return pUser;
	/*
	record::RecordFieldListRole all;
	if (!cRedisMgr::get().opReadListRole(zoneIdF, roleId, all))
	{
		return nullptr;
	}
	*/
	pUser = new TeamUser(roleId);
	if (pUser == nullptr)
	{
		Log_Error("pUser==nullptr,zoneIdF:%u,%lu", zoneIdF, roleId);
		return nullptr;
	}
	/*
	if (!pUser->unserialize(std::move(all)))
	{
		return nullptr;
	}
	*/
	pUser->setZoneIdF(zoneIdF);
	if (!addRole(pUser))
	{
		Log_Error("!addRole.zoneIdF:%u,%lu", zoneIdF, roleId);
		pUser->final();
		SAFE_DELETE(pUser);
		return nullptr;
	}
	Log_Info("tryCreateRole.%u,%lu", zoneIdF, roleId);
}