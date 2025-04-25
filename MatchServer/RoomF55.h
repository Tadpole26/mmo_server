#pragma once

#include "MatchRoom.h"

class QueueMgrF55;
class RoomF55 : public MatchRoom
{
public:
	struct ExtendInfo
	{
		uint32 totalRealScore = 0;
		uint32 avgRealScore = 0;
	};
public:
	ExtendInfo redInfo;
	ExtendInfo blueInfo;
public:
	RoomF55(zRoomIdType roomId, std::string&& fromStr, MatchQueueMgr& mgr) : MatchRoom(roomId, std::move(fromStr), mgr) {}
	virtual ~RoomF55() {}
	void final() override { MatchRoom::final(); }
	void addGroup(MatchGroup& group) override;
	void fill(inner::InnerCopyTeamList& out) override;
	void fill(inner::InnerCopyType& out) override;
public:
	QueueMgrF55& mgrF55Ref();
};
