#pragma once

#include "singleton.h"

namespace inner
{
	class InnerCopyCreateBy;
}

class MatchRoom;
class MatchQueueMgr;
class MatchRoomMgr
{
public:
	using RoomIdPairT = std::pair<uint32, uint32>;
	using RecycleListT = std::list<MatchRoom*>;
	using RoomMapT = std::unordered_map<zRoomIdType, MatchRoom*>;
	using MgrIdMapT = std::unordered_map<uint64, RoomMapT>;
	using FuncRoomT = std::function<bool(MatchRoom&)>;
public:
	MatchRoomMgr();
	~MatchRoomMgr();
	void final();
	void tick();
public:
	size_t sizeRoom() { return _alls.size(); }
	size_t sizeRecycle() { return _recycles.size(); }
	MatchRoom* createRoom(MatchQueueMgr& mgr, std::string&& formStr);
	MatchRoom* getRoom(zRoomIdType roomId);
	void addToRecycle(MatchRoom* pRoom);
	void copyCreateResult(zSceneIdType sceneId, uint32 hashId, const inner::InnerCopyCreateBy& createBy);
private:
	void __recycle();
	void __autoIdPair();
private:
	RoomIdPairT _idpair = {0, 0};
	RecycleListT _recycles;
	RoomMapT _alls;
};

#define gMatchRoomMgr Singleton<MatchRoomMgr>::getInstance()
