#include "innermatch.pb.h"
#include "innercommon.pb.h"

#include "MatchMem.h"
#include "MemRoleF55.h"

MemRoleF55::MemRoleF55(zRoleIdType roleId, const inner::InnerMatchMatcher& in) : 
	MemRole(roleId, in)
{
}

MemRoleF55::~MemRoleF55()
{
}

void MemRoleF55::fill(common::MatchFighterExtend& out)
{
	*out.mutable_fight5v5() = _info->extd().fight5v5();
}

uint32 MemRoleF55::getMatchScore()
{
	return _info->extd().fight5v5().score();
}

uint32 MemRoleF55::getRealScore()
{
	return _info->extd().fight5v5().score();
}

uint32 MemRoleF55::getCLoseF55()
{
	return _info->extd().fight5v5().base().close();
}