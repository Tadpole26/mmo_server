#pragma once
#include "sdk.h"
#include "cTeam.h"
#include "TeamDefine.h"

namespace common
{
	class PlatTargetTeam;
	class TeamBase;
	class TeamMemberList;
	class TeamFieldList;
	class RecruitTeam;
}

namespace inner
{
	class InnerTeam;
	class InnerTeamBase;
	class InnerTeamField;
}

class TeamUser;
class TeamMember;
class TeamLeader;
class SessionTeam
{
public:
	//static bool validIndex(uint32 index);
public:
	using MemberListT = std::list<TeamMember *>;
	using MemberIndexListT = std::list <uint32>;
	using SizeQueueTargetPairT = std::pair<TeamSizeQueueTarget*, TeamSizeQueueItT>;		
	using SizeQueueLevelPairT = std::pair<TeamSizeQueueLevel*, TeamSizeQueueItT>;
	using SizeQueueLevelListT = std::list<SizeQueueLevelPairT>;
public:
	SessionTeam(zTeamIdType teamId);
	~SessionTeam();
	void final();
public:
	void fill(common::PlatTargetTeam& out);
	void fill(common::TeamBase& out);
	void fill(common::TeamMemberList& out);
	void fill(common::TeamFieldList& out, uint32 fieldId);
	void fill(common::RecruitTeam& out);
	bool broadRecruitToWorld();
	bool sendCmdToTeam(const uint32 first, const uint32 second, const google::protobuf::Message& in);
	bool sendCmdToTeamExcept(TeamMember* pExcept, const uint32 first, const uint32 second, const google::protobuf::Message& in);
	bool sendChgFieldToTeam(std::initializer_list<uint32> changes);
public:
	void fill(inner::InnerTeam& out);
	void fill(inner::InnerTeamBase& out);
	void fill(inner::InnerTeamField& out, uint32 fieldId);
	bool broadChgFieldToScene(std::initializer_list<uint32> changes);
	bool broadAddMemeberToScene(TeamMember* pMember);
	bool broadRemoveMemberToScene(zRoleIdType roleId, uint32 type);
public:
	bool isMemberEmpty();
	bool isMemberFull();
	bool isLeader(TeamMember* pMember);
	bool isLeader(TeamUser* pUser);
	bool isLeader(zRoleIdType roleId);
	bool isMember(TeamMember* pMember);
	bool isMember(TeamUser* pUser);
	bool isMember(zRoleIdType roleId);
	bool isMatching();
	bool isVoting();
	bool isRecruitExpire();
	bool checkMidwayJoin(zSceneIdType leaderSceneId = 0);
public:
	TeamLeader& teamLeaderRef() { return *_leader; }
	zTeamIdType getTeamId() { return _teamId; }
	uint32 sizeMember() { return _members.size(); }
	uint32 getMinLevel() { return _minLevel; }
	uint32 getMaxLevel() { return _maxLevel; }
	uint32 getMatchTime() { return _matchTime; }
	std::string& getText() { return _text; }
	uint32 getMidwayJoin() { return _midwayJoin; }
	uint32 getTargetId() { return _targetId; }
	zRoleIdType getLeaderId();
	TeamMember* getMember(zRoleIdType roleId);
	TeamMember* leaderPtr();
public:
	void createLeader(TeamUser* pUser);
	void createMember(TeamUser* pUser);
	void setTarget(uint32 targetId);
	void setMinLevel(uint32 minLevel);
	void setMaxLevel(uint32 maxLevel);
	void setText(const std::string& text);
	void setMatchTime();
	void resetMatchTime();
	void setRecruitExpire();
	void resetRecruitExpire();
	void setMidwayJoin(uint32 join);
public:
	bool foreachMem(std::function<bool(TeamMember& memRef)> func);
	bool foreachMemLess(std::function<bool(TeamMember& memRef)> func);
	void starMatch();
	void cancelMatch();
	void leaveMemberLast();
	void leaveMember(TeamMember* pMember);
	void fireMember(TeamMember* pMember);
	void changeMemberIndex(uint32 index);
	void cancelVote();
public:
	void addToSzQueueTarget();							//队长创建队伍,就加入显示面板
	void removeFromSzQueueTarget();
	void addToSzQueueLevelAll();						//队长开启招募队友加入匹配队列
	void removeFromSzQueueLevelAll();
private:
	TeamMember* __createMember(TeamUser* pUser);
	void __addMember(TeamMember* pMember, uint32 index);
	void __removeMember(TeamMember* pMember);
public:
	SizeQueueTargetPairT szQueueTargetPair;				//team对应target队列
	SizeQueueLevelListT szQueueLevelList;				//team对应等级区间,所以一个匹配队伍对应多个队列中
private:
	zTeamIdType _teamId = 0;
	uint32 _targetId = 0;								//队伍目标(pve副本类型)
	MemberListT _members;
	MemberIndexListT _indexs;
	TeamLeader* _leader = nullptr;
	uint32 _minLevel = 0;
	uint32 _maxLevel = 0;
	uint32 _matchTime = 0;					
	std::string _text = "";
	uint32 _recruitExpire = 0;
	uint32 _midwayJoin = 1;
};