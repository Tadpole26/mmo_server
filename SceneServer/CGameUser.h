#pragma once
#include "sdk.h"
#include "CUser.h"

class CGameUser : public CUser
{
public:
	CGameUser(const uint64 ullRoleId);
private:
	virtual ~CGameUser();
	virtual void Final();
};
