#pragma once
#include "sdk.h"

namespace common
{
	class TeamApply;
}

class TeamUser;
class TeamApply
{
public:
	TeamUser* pUser = nullptr;
	uint32 expire = 0;
	zSceneIdType leaderSceneId = 0;
public:
	TeamApply(TeamUser* pUser, zSceneIdType leaderSceneId);
	~TeamApply();
	void final();
	void fill(common::TeamApply& out);
	bool isExpire();
};