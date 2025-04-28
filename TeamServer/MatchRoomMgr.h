#pragma once

#include "cMatch.h"
#include "singleton.h"
#include "matchcommon.pb.h"

namespace inner
{
	class InnerMatchGroup;
	class InnerMatchRoom;
	class InnerCopyCreateBy;
	class InnerMatchRoomCopyScene;
}

class MatchUnit;
class MatchRoom
{
public:
	using UnitListT = std::list<MatchUnit*>;
	using ConfirmListT = std::list<zRoleIdType>;
public:
	MatchRoom(const inner::InnerMatchRoom& in);
	~MatchRoom();
	void final();
public:
	void evMatchStartConfirm();
	void evMatchStartLoading();
public:
	void fill(common::MatchGroupList& out);
	common::MatchGroup group2group(const inner::InnerMatchGroup& in);
public:
	uint32 sizeUnit() { return _allunit.size(); }
	uint32 sizeConfirm() { return _confirms.size(); }
	bool emptyUnit() { return _allunit.empty(); }
public:
	uint32 getMatchType();
	zRoomIdType getRoomId();
	const inner::InnerCopyCreateBy& getCreateBy() { return *_createby; }
	const inner::InnerMatchRoomCopyScene& getCopyScene() { return *_copyscene; }
	bool isExpire();
	bool isConfirmed(zRoleIdType roleId);
public:
	bool foreach(std::function<bool(MatchUnit& unit)> func);
	bool foreach(uint32 groupId, std::function<bool(MatchUnit& unit)> func);
	MatchUnit* getUnit(zUnitIdType unitId);
	void addUnit(MatchUnit* pUnit);
	void removeUnit(MatchUnit* pUnit);
	void setCopyScene(const inner::InnerMatchRoomCopyScene& in);
	void setCreateBy(const inner::InnerCopyCreateBy& in);
public:
	void startConfirm(uint32 expire);
	void setConfirmOk(zRoleIdType roleId);
	void startLoading();
	void forceRecycle();
private:
	UnitListT _allunit;
	ConfirmListT _confirms;
	inner::InnerMatchRoom* _room = nullptr;
	inner::InnerMatchRoomCopyScene* _copyscene = nullptr;
	inner::InnerCopyCreateBy* _createby = nullptr;
	uint32 _expire = 0;
};

class MatchRoomMgr
{
public:
	using RoomMapT = std::unordered_map<zRoomIdType, MatchRoom*>;
	using RoomListT = std::list<MatchRoom*>;
public:
	MatchRoomMgr();
	~MatchRoomMgr();
	void final();
	void tick();
public:
	size_t sizeRoom() { return _mrooms.size(); }
	size_t sizeRecycle() { return _rmrooms.size(); }
	MatchRoom* createRoom(const inner::InnerMatchRoom& in);
	MatchRoom* getRoom(zRoomIdType roomId);
	void addToRecycle(MatchRoom* pRoom);
	void forceRecycle();
private:
	void __recycle();
private:
	RoomMapT _mrooms;
	RoomListT _rmrooms;
};

#define gMatchRoomMgr Singleton<MatchRoomMgr>::getInstance()