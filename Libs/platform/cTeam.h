#pragma once

#include "sdk.h"
#include "cServer.h"

using zVoteIdType = uint32;
using zTeamIdType = uint64;

namespace cTeam
{
	//合成队伍id
	inline zTeamIdType unionTeamId(uint32 threeId, uint32 timestamp, uint32 autoId)
	{
		assert(autoId <= (1 << 13));
		return ((uint64)threeId <<46) + ((uint64)timestamp << 14) + ((uint64)autoId);
	}
	inline uint32 splitThreeIdTeamId(zTeamIdType teamId)
	{
		return teamId >> 46;
	}
	inline uint32 splitEprocTeamId(zTeamIdType teamId)
	{
		return splitEprocThreeId(splitThreeIdTeamId(teamId));
	}
	inline uint32 splitEparamTeamId(zTeamIdType teamId)
	{
		return splitEparamThreeId(splitThreeIdTeamId(teamId));
	}
	enum
	{
		eJumpToLeaderNone = 0,
		eJumpToLeaderJoin,
		eJumpToLeaderFollow,
		eJumpToLeaderTogether,
	};
	enum
	{
		eEnterByNone = 0,
		eEnterByMatch,	//匹配(队员投票)
		eEtnerByHome,	//家园方式进入(直接进入)
		eEnterByTeam,	//组队进入(队员投票)
	};
}
