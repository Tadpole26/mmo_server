#include "GateUser.h"
#include "GateUserMgr.h"

GateUserMgr::GateUserMgr()
{
}

GateUserMgr::~GateUserMgr()
{
}

bool GateUserMgr::init()
{
	return true;
}

void GateUserMgr::final()
{
	for (auto it = _alluser.begin(); it != _alluser.end();)
	{
		auto del = it++;
		auto* pUser = del->second;
		pUser->final();
		SAFE_DELETE(pUser);
	}
}

void GateUserMgr::tick(uint32 tickSec)
{
}

GateUser* GateUserMgr::createRole(zRoleIdType roleId)
{
	if (!roleId)
	{
		Log_Error("createRole!roleId");
		return nullptr;
	}
	auto* pUser = new GateUser(roleId);
	if (!pUser)
	{
		Log_Error("createRole.!pUser");
		return nullptr;
	}
	if (!_alluser.emplace(roleId, pUser).second)
	{
		Log_Error("createRole.!emplace.%lu", roleId);
		return nullptr;
	}
	Log_Info("createRole.%lu", roleId);
	return pUser;
}

GateUser* GateUserMgr::getRole(zRoleIdType roleId)
{
	auto it = _alluser.find(roleId);
	if (it == _alluser.end())
	{
		return nullptr;
	}
	return it->second;
}

void GateUserMgr::destroyRole(GateUser* role)
{
	auto it = _alluser.find(role->getRoleId());
	if (it != _alluser.end())
	{
		_alluser.erase(it);
		role->final();
		SAFE_DELETE(role);
	}
}