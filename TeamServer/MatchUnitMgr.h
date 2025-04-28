#pragma once

#include "cMatch.h"
#include "singleton.h"

namespace common
{
	class MatchStatus;
}

namespace inner
{
	class InnerMatchPunishList;
}

class TeamUser;
class MatchRoom;
class MatchUnit
{
public:
	using MemberListT = std::list<TeamUser*>;
public:
	MatchUnit(zUnitIdType unitId, uint32 mtype);
	~MatchUnit();
	void final();
public:
	void fill(common::MatchStatus& out);
public:
	zUnitIdType getUnitId() { return _unitId; }
	uint32 getMatchType() { return _mtype; }
	uint32 getMatchTime() { return _mtime; }
	uint32 getGroupId() { return _groupId; }
	uint32 sizeMember() { return _allmember.size(); }
	bool emptyMember() { return _allmember.empty(); }
	void setGroupId(uint32 groupId) { _groupId = groupId; }
public:
	bool foreach(std::function<bool(TeamUser& member)> func);
	void addMember(TeamUser* pUser);
	void removeAllMember();
public:
	void startMatch();
	void restartMatch();
	void restartMatchFromQueue();
	void cancelMatch();
	void punishMatch(const inner::InnerMatchPunishList& in);
	void startConfirm();
	void startLoading();
	void forceCancelWithEv();
	void forceCancel();
public:
	MatchRoom* room = nullptr;
private:
	zUnitIdType _unitId = 0;
	uint32 _mtype = 0;
	uint32 _mtime = 0;
	uint32 _groupId = 0;
	MemberListT _allmember;
};

class MatchUnitMgr
{
public:
	using UnitMapT = std::unordered_map<zUnitIdType, MatchUnit*>;
	using UnitListT = std::list<MatchUnit*>;
public:
	MatchUnitMgr();
	~MatchUnitMgr();
	void final();
	void tick();
public:
	uint32 sizeUnit() { return _munits.size(); }
	uint32 sizeRecycle() { return _rmunits.size(); }
	MatchUnit* createUnit(zUnitIdType unitId, uint32 mtype);
	MatchUnit* getUnit(zUnitIdType unitId);
	void addToRecycle(MatchUnit* pUnit);
	void forceCancelWithEv();
	void forceCancel();
private:
	void __recycle();
private:
	UnitMapT _munits;
	UnitListT _rmunits;
};

#define gMatchUnitMgr Singleton<MatchUnitMgr>::getInstance()
