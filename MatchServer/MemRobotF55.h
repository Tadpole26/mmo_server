#pragma once
#include "MatchMem.h"

class MemRobotF55 : public MemRobot
{
public:
	MemRobotF55(zRoleIdType robotId, const inner::InnerMatchMatcher& in, const config::robot_info_t& cfg);
	virtual ~MemRobotF55();
	void fill(common::MatchFighterExtend& out) override;
	uint32 getMatchScore();
	uint32 getRealScore();
	uint32 getCLose();
};
