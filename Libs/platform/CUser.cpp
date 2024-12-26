#include "CUser.h"
#include "CGame.h"

CUser::CUser(const uint64 ullRoleId) : 
	m_ullRoleId(ullRoleId)
{
}

CUser::~CUser()
{
}

void CUser::Final()
{
}

uint64 CUser::GetAccId() const
{
	return CGame::RoleId2AccId(m_ullRoleId);
}

uint32 CUser::GetDBId() const
{
	return CGame::RoleId2DBId(m_ullRoleId);
}

uint32 CUser::GetTableId() const
{
	return CGame::RoleId2TableId(m_ullRoleId);
}

uint32 CUser::GetZoneIdB() const
{
	return CGame::RoleId2ZoneId(m_ullRoleId);
}

uint32 CUser::GetRoleNumId() const
{
	return CGame::RoleId2RoleNumId(m_ullRoleId);
}

void CUser::SetNickName(const std::string& strNickName)
{
	m_strNick = strNickName;
}

void CUser::SetZoneIdF(uint32 uiZoneIdF)
{
	m_usZoneIdFrom = uiZoneIdF;
}