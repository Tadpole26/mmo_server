#include "cUser.h"
#include "cGame.h"

cUser::cUser(const zRoleIdType roleId) :
	_roleId(roleId)
{
}

cUser::~cUser()
{
}

void cUser::final()
{
}

zAccIdType cUser::getAccId() const
{
	return cGame::RoleId2AccId(_roleId);
}

uint32 cUser::getDBId() const
{
	return cGame::RoleId2DBId(_roleId);
}

uint32 cUser::getTableId() const
{
	return cGame::RoleId2TableId(_roleId);
}

uint32 cUser::getZoneIdB() const
{
	return cGame::RoleId2ZoneId(_roleId);
}

uint32 cUser::getRoleNumId() const
{
	return cGame::RoleId2RoleNumId(_roleId);
}

void cUser::setNickName(const std::string& nickName)
{
	_nickName = nickName;
}

void cUser::setZoneIdF(uint32 uiZoneIdF)
{
	_zoneId = uiZoneIdF;
}