#include "innermatch.pb.h"
#include "innercommon.pb.h"
#include "matchcommon.pb.h"

#include "MemRobotF55.h"

MemRobotF55::MemRobotF55(zRoleIdType robotId, const inner::InnerMatchMatcher& in, const config::robot_info_t& cfg) :
	MemRobot(robotId, in, cfg)
{
}

MemRobotF55::~MemRobotF55()
{
}

void MemRobotF55::fill(common::MatchFighterExtend& out)
{
	*out.mutable_fight5v5() = _info->extd().fight5v5();
}

uint32 MemRobotF55::getMatchScore()
{
	return _info->extd().fight5v5().score();
}

uint32 MemRobotF55::getRealScore()
{
	return _info->extd().fight5v5().score();
}

uint32 MemRobotF55::getCLose()
{
	return _info->extd().fight5v5().base().close();
}