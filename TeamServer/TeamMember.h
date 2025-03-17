#pragma once

#include "sdk.h"

namespace common
{
	class PlatTeamMember;
	class TeamMember;
	class RecruitTeamMember;
	class TeamMemberFieldList;
}

class TeamUser;
class SessionTeam;
class TeamMember
{
public:
	TeamMember();
	~TeamMember();
	void final();
	//bool sendCmdToMe();
	bool sendChgFieldToMe(std::initializer_list<uint32> changes);
	bool sendChgFieldToTeam(std::initializer_list<uint32> changes);
	void fill(common::PlatTeamMember& out);
	void fill(common::TeamMember& out);
	void fill(common::RecruitTeamMember& out);
	void fillShare(common::TeamMemberFieldList& out, uint32 fieldId);
	void fillSelf(common::TeamMemberFieldList& out, uint32 fieldId);
public:
	bool broadChgFieldToScene(std::initializer_list<uint32> changes);
	//void fill(inner::InnerTeamMemeber);
public:
	inline SessionTeam* teamPtr() { return _team; }
	inline SessionTeam& teamRef() { return *_team; }
	inline uint32 getExpireReplace() { return _replace; }
	inline uint32 getExpireInvite() { return _invite; }
	inline uint32 getExpireTogether() { return _together; }
	inline uint32 getExpireAppoint() { return _appoint; }
	inline uint32 getIndex() { return _index; }
	inline bool getState() { return _state; }
	inline bool getFollow() { return _follow; }
	inline bool isFollow() { return _follow; }
	inline bool isOnline() { return _state; }
	bool isLeader(); 
	bool isExpireReplace();
	bool isExpireInvite();
	bool isExpireTogether();
	bool isExpireAppoint();
	void setIndex(uint32 index);
	void setOnline();
	void setFollow();
	void setExpireReplace();
	void setExpireInvite();
	void setExpireTogether();
	void setExpireAppoint();
	void resetOnline();
	void resetFollow();
	void resetExpireReplace();
	void resetExpireInvite();
	void resetExpireTogether();
	void resetExpireAppoint();
	void tryExpireRemove();
	void jumpToLeader(uint32 eJumpTo, uint32 sceneHashId);
private:
	TeamUser* pUser = nullptr;
private:
	SessionTeam* _team = nullptr;
	bool _state = true;						//是否在线
	bool _follow = false;					//是否跟随
	uint32 _replace = 0;					//替换队长时间
	uint32 _invite = 0;						//邀请时间
	uint32 _together = 0;					//召集时间
	uint32 _appoint = 0;					//任命时间
	uint32 _index = 0;						//所在队伍索引位置
};