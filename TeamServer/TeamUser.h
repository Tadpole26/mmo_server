#pragma once
#include "sdk.h"
#include "cUser.h"
#include "cTeam.h"
#include "TeamDefine.h"

namespace inner
{
	class InnerGameEvent;
	class InnerCopyPve;
	class InnerCopyEnter;
	class InnterCopyType;
}

namespace common
{
	class RoleBrief;
	class RoleExtend1;
	class RoleExtend2;
	class RoleExtend3;
	class ExtendFight3v3;
	class ExtendFight5v5;
	class NotOwnTeamInfo;
	class OwnTeamInfo;
	class TeamInvited;
	class TeamMemberFieldList;
}

namespace config
{
	struct team_info_t;
}

class TeamMember;
class MatchUnit;
class TeamLevelQueue;
class LevelQueuePairT;
class TeamUser : public cUser
{
public:
	using LevelQueuePairT = std::pair<TeamLevelQueue*, TeamSingleListItT>;
public:
	TeamUser(const zRoleIdType roleId);
	virtual ~TeamUser();
	void final() override;
private:
	uint32 _sceneHashId = 0;
	common::RoleBrief* _brief = nullptr;
	common::RoleExtend1* _extd1 = nullptr;
	common::RoleExtend2* _extd2 = nullptr;
	common::RoleExtend3* _extd3 = nullptr;
	common::ExtendFight3v3* _f33 = nullptr;
	common::ExtendFight5v5* _f55 = nullptr;
private:											//被邀请
	TeamUser* _inviteUser = nullptr;
	uint32 _inviteExpire = 0;
	zTeamIdType _inviteTeamId = 0;
	zSceneIdType _inviteLeaderSceneId = 0;
private:
	uint32 _apply = 0;								//主动申请(过期时间)
	uint32 _create = 0;								//创建队伍(过期时间)	
	uint32 _match = 0;								//主动匹配队伍
	TeamVote* _vote = nullptr;						//投票中
public:
	LevelQueuePairT lvQueuePair;					//是否个人匹配队伍
	TeamMember* pMem = nullptr;						//队伍成员
	MatchUnit* pUnit = nullptr;						//当前正在pvp匹配
public:
	uint32 _punishExpire = 0;
public:
	TeamVote* votePtr() const { return _vote; }
	bool isVoting() const { return votePtr() != nullptr; }
	void setVotePtr(TeamVote* vote);
public:
	void fill(common::RoleBrief& out);
	void fill(common::RoleExtend1& out);
	void fill(common::RoleExtend2& out);
	void fill(common::RoleExtend3& out);
	void fill(common::ExtendFight3v3& out);
	void fill(common::ExtendFight5v5& out);
public:
	void fill(common::NotOwnTeamInfo& out);
	void fill(common::OwnTeamInfo& out);
	void fill(common::TeamInvited& out);
	void fill(common::TeamMemberFieldList& out);
public:
	const std::string getMapInfoByte();
	const std::string getMapPosByte();
	const std::string getModelByte();
	const std::string getFashionByte();
	const std::string getMainHandByte();
	const std::string getOffHandByte();
	uint32 getHeadIcon();
	uint32 getHeadFrame();
	uint32 getCareerId();
	uint32 getLevel();
	uint32 getCareerLevel();
	uint32 getHp();
	uint32 getMp();
	uint32 getMaxHp();
	uint32 getMaxMp();
	uint32 getSex();
	uint32 getFightPoint();
	uint32 getChatFont();
	uint32 getChatFrame();
	uint32 getEproc() { return 0; }
	uint32 getSceneHashId() { return _sceneHashId; }
	zSceneIdType getSceneId();
public:
	void setSceneHashId(uint32 hashId);
	void setBrief(const common::RoleBrief& in);
	void setExtd1(const common::RoleExtend1& in);
	void setExtd2(const common::RoleExtend2& in);
	void setExtd3(const common::RoleExtend3& in);
	void setF33(const common::ExtendFight3v3& in);
	void setF55(const common::ExtendFight5v5& in);
public:
	//被邀请
	void resetInvite();
	void setInvite(TeamUser* pUser);
	bool hasInvited() { return _inviteUser != nullptr; }
	TeamUser& inviteUserRef() { return *_inviteUser; }
	zTeamIdType inviteTeamId() { return _inviteTeamId; }
	uint32 inviteExpire() { return _inviteExpire; }
	bool isExpireInvite();
	//主动申请
	void resetExpireApply();
	void setExpireApply();
	bool isExpireApply();
	uint32 getExpireApply() { return _apply; }
	//创建队伍
	void resetExpireCreate();
	void setExpireCreate();
	bool isExpireCreate();
	uint32 getExpireCreate() { return _create; }
public:
	//个人匹配队伍
	uint32 getMatchTime() { return _match; }
	void setMatchTime();
	void resetMatchTime();
	bool isTeamPersonMatching();
	void tryExpireRemove();
	void addToLvQueue(const config::team_info_t& cfg);
	void removeFromLvQueue();
	void startMatchPerson(const config::team_info_t& cfg);
	void cancelMatchPerson();
public:
	void evGameDestroy(const inner::InnerGameEvent& in);
	void evGameLogin(const inner::InnerGameEvent& in);
	void evGameLogout(const inner::InnerGameEvent& in);
	void evGameOnline(const inner::InnerGameEvent& in);
	void evGameOffline(const inner::InnerGameEvent& in);
	void evGameDeath(const inner::InnerGameEvent& in);
	void evGameRebirth(const inner::InnerGameEvent& in);
	void evGameWakeup(const inner::InnerGameEvent& in);
	void evGameChangeMap(const inner::InnerGameEvent& in);
public:
	void mountTeam();
	void leaveTeam(uint32 eLeave);
public:
	void mountMatch();
	void forceCancelToMatch();
	bool isPunishExpire();
	uint32 getPunishExpire();
	void setMatchPunish(uint32 ePunish);
	void openMatchPanelNtf();
	void closeMatchPanelNtf();
	void closeConfirmPanelNtf();
public:
	bool innerMatchStartMatch(const inner::InnterCopyType& in, uint32 mtype);
	bool innerMatchCancelMatch();
	bool innerMatchConfirmOk();
	bool innerMatchRefuseOk();
	bool innerMatchPunish(uint32 ePunish);
	bool innerMatchOpCancel();
public:
	bool innerPersonChangeTarget(uint32 targetId);
	bool innerPersonCancelMatch();
	bool innerLeaderStartMatch(const std::string& text, uint32 languageId, uint32 teamId = 0);
	bool innerLeaderCancelMatch();
	bool innerCreateTeam(uint32 targetId, uint32 minLevel, uint32 maxLevel);
	bool innerLeaderChangeTarget(uint32 targetId, uint32 minLevel, uint32 maxLevel);
	bool innerLeaderFireMember(zRoleIdType roleId);
	bool innerLeaderTogetherMember(zRoleIdType roleId);
	bool innerleaderAppoint(zRoleIdType roleId);
	bool innerLeaderRefuseAllApply();
	bool innerLeaderAgreeAllApply();
	bool innerLeaderRefuseApply(uint32 zoneIdF, zRoleIdType roleId);
	bool innerLeaderAgreeApply(uint32 zoneIdF, zRoleIdType roleId);
	bool innerInviteFriend(zRoleIdType roleId);
	bool innerInviteFamily(zRoleIdType roleId);
	bool innerInvitePlayer(uint32 zoneIdF, zRoleIdType roleId);
	bool innerAgreeInvited(uint32 sceneHashId);
	bool innerRefuseInvited();
	bool innerApplyTeam(uint32 sceneHashId, zTeamIdType teamId, zSceneIdType leaderSceneId);
	bool innerFollowLeader();
	bool innerCancelFollow();
	bool innerAgreeTogether(uint32 sceneHashId);
	bool innerRefuseTogether();
	bool innerReplaceLeader();
	bool innerJumpToLeader(uint32 sceneHashId, uint32 eJumpTo);
	bool innerEnterByVote(const inner::InnerCopyPve& pve, const inner::InnerCopyEnter& enter);
	bool innerPersonVote(bool agree);
};