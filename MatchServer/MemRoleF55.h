#pragma once

#include "MatchMem.h"
class MemRoleF55 : public MemRole
{
public:
	MemRoleF55(zRoleIdType roleId, const inner::InnerMatchMatcher& in);
	~MemRoleF55();
	void fill(common::MatchFighterExtend& out);
	uint32 getMatchScore();
	uint32 getRealScore();
	uint32 getCLoseF55();
};
