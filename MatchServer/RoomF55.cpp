#include "innercommon.pb.h"

#include "cServer.h"

#include "MatchMem.h"
#include "RoomF55.h"
#include "MatchGroup.h"
#include "QueueMgrF55.h"

void RoomF55::addGroup(MatchGroup& group)
{
	MatchRoom::addGroup(group);
	auto& groupF55 = static_cast<GroupF55&>(group);
	if (groupF55.getGroupId() == cMatch::eRoomGroupIdRed)
	{
		redInfo.totalRealScore = groupF55.getTotalRealScore();
		redInfo.avgRealScore = groupF55.getAvgRealScore();
	}
	else if (groupF55.getGroupId() == cMatch::eRoomGroupIdBlue)
	{
		blueInfo.totalRealScore = groupF55.getTotalRealScore();
		blueInfo.avgRealScore = groupF55.getAvgRealScore();
	}
	else assert(false);
}

void RoomF55::fill(inner::InnerCopyTeamList& out)
{
	//out.set_targetid(mgrF55Ref().teamID);
	cMatch::foreachRoomGroup([&](auto groupId)->bool
		{
			auto& team = *out.add_teams();
			return foreachUnit(groupId, [&](MatchUnit& unit) ->bool
			{
				unit.foreachRole([&](MemRole& role)
				{
					role.fill(*team.add_roles());
					return true;
				});
				unit.foreachRobot([&](MemRobot& robot)
				{
					robot.fill(*team.add_robots());
					return true;
				});
				return true;
			});
			return true;
		});
}

void RoomF55::fill(inner::InnerCopyType& out)
{
	auto& copy = out;
	//copy.set_copylvid();
	copy.set_eproc(mgrF55Ref().getEproc());
	switch (mgrF55Ref().getEproc())
	{
	case eProcThreeLocal:
		copy.set_threeid(unionThreeId(eProcThreeGroup, 1));					//groupId TODO
		break;
	default: 
		copy.set_threeid(unionThreeId(mgrF55Ref().getEproc(), 1));			//TODO
		break;
	}
	auto& pvp = *copy.mutable_pvp();
	auto funcF55 = ([&](inner::InnerCopyPvpF55& out) 
	{
		out.set_copyid(mgrF55Ref().getMatchId());
	});
	switch (mgrF55Ref().getMtype())
	{
	case cMatch::eMatchF55: funcF55(*pvp.mutable_f55());			break;
	default:	assert(false);										break;
	}
}

QueueMgrF55& RoomF55::mgrF55Ref()
{
	return static_cast<QueueMgrF55&>(mgrRef());
}