#pragma once
#include "sdk.h"

class CUser
{
public:
	CUser(const uint64 ullRoleId);
	~CUser();
	void Final();
public:
	uint16 GetZoneId() { return m_usZoneIdFrom; }
	uint16 GetGroupId() { return m_usGroupId; }
	uint64 GetRoleId() { return m_ullRoleId; }
	const std::string& GetNickName() { return m_strNick; }
	uint64 GetAccId() const;
	uint32 GetDBId() const;
	uint32 GetTableId() const;
	uint32 GetZoneIdB() const;
	uint32 GetRoleNumId() const;
	void SetNickName(const std::string& strNickName);
	void SetZoneIdF(uint32 uiZoneIdF);
private:
	uint16 m_usZoneIdFrom = 0;							//来自哪个区(scenesvr有可能为副本服)
	uint16 m_usGroupId = 0;
	uint64 m_ullRoleId = 0;
	std::string m_strNick;
};
