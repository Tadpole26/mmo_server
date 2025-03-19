#pragma once

#include "sdk.h"

namespace common
{
	class TeamApplyList;
	class RecruitTeamLeader;
}

class TeamApply;
class TeamUser;
class TeamMember;
class TeamLeader
{
public:
	using ApplyListT = std::list<TeamApply*>;				//list保证申请顺序,所以不用unordered_map<role,apply>
	using ApplyListItT = ApplyListT::iterator;
	using ApplyMapT = std::unordered_map<TeamUser*, ApplyListItT>;
public:
	TeamLeader(TeamMember* pMember);
	~TeamLeader();
	void final();
public:
	//bool sendCmdToMe();
public:
	uint32 getSceneHashId();
	zSceneIdType getSceneId();
	inline TeamMember* memberPtr() { return _pMem; }
	inline TeamMember& memberRef() { return *_pMem; }
	inline void resetMember() { _pMem = nullptr; }
	inline uint32 sizeApply() { return _applyList.size(); }
	inline bool applyEmpty() { return _applyList.empty(); }
	bool isApplyFull();
	bool hasApply(TeamUser* pUser);
public:
	void fill(common::TeamApplyList &out);
	void fill(common::RecruitTeamLeader &out);
	bool foreachApply(std::function<bool(TeamApply* pApply)> func);
public:
	void createApply(TeamUser* pUser);
	void deleteApply(TeamUser* pUser);
	void deleteApplyAll();
	void tryExpireRemove();
	void agreeApply(TeamUser* pUser);
	void refuseApply(TeamUser* pUser);
	void togetherMember(TeamMember* pMember);
	void appointLeader(TeamMember* pMember);
	void replaceLeader(TeamMember* pMember);
private:
	void __addApply(TeamApply* pApply);
	void __deleteApply(TeamUser *pUser);
	void __deleteApplyAll();
private:
	TeamMember* _pMem = nullptr;
	ApplyListT _applyList;
	ApplyMapT _applyMap;
};
