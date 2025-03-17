#pragma once
#include "sdk.h"

class cUser
{
private:
	zRoleIdType _roleId = 0;
	std::string _nickName;
	uint32 _zoneId = 0;
public:
	cUser(const uint64 ullRoleId);
	~cUser();
	void final();
public:
	const std::string& getNickName() { return _nickName; }
	zRoleIdType getRoleId() { return _roleId; }
	zAccIdType getAccId() const;
	uint32 getDBId() const;
	uint32 getTableId() const;
	uint32 getZoneIdB() const;
	uint32 getRoleNumId() const;
	uint32 getZoneIdF() const;
	uint32 getGroupIdF() const;
public:
	void setNickName(const std::string& nickName);
	void setZoneIdF(uint32 zoneIdF);
};
