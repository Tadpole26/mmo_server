#pragma once

#include "cTeam.h"
#include "TeamDefine.h"

namespace common
{
	class VoteBaseData;
	class RoleVoteState;
	class RoleVoteStateList;
}

namespace inner
{
	class InnerCopyCrit;
	class InnerCopyEnter;
	class InnerCopyType;
	class InnerCopyWay;
}

enum
{
	VoteState_None = 0,
	VoteState_Check = 1,
	VoteState_Vote = 2,
	VoteState_Confirm = 3,
	VoteState_Over = 4,
};

enum
{
	VoteMethod_None = 0,
	VoteMethod_Agree = 1,
	VoteMethod_Oppose = 2,
	VoteMethod_GiveUp = 3
};

enum
{
	VoteResult_None = 0,
	VoteResult_Pass = 1,
	VoteResult_Reject = 2,
	VoteResult_CheckFail = 3
};

class TeamUser;
class VoteUserData
{
public:
	explicit VoteUserData(zRoleIdType roleId) : roleId(roleId) { }
	~VoteUserData();
public:
	void fill(common::RoleVoteState& out) const;
public:
	zRoleIdType roleId = 0;
	uint32 method = VoteMethod_None;							//ͨ��/�ܾ�
	uint32 checked = 0;
	bool right = true;											//�Ƿ���Բ���ͶƱ
};

class TeamVote
{
public:
	using VoteCheckHandle = std::function<void()>;				//ͶƱ�м��ص�
	using VoteCheckFailHandle = std::function<void()>;
	using VoteBeginHandle = std::function<void()>;
	using VoteUpdateHandle = std::function<void(const VoteUserData&)>;
	using VotePassHandle = std::function<void()>;
	using VoteRejectHandle = std::function<void()>;
public:
	explicit TeamVote(zVoteIdType uid);
	virtual ~TeamVote();
	virtual void final();
public:
	void fill(common::VoteBaseData& out) const;
	void fill(common::RoleVoteStateList& out) const;
	void fill(inner::InnerCopyCrit& out) const;
	void fill(const inner::InnerCopyEnter& enter, inner::InnerCopyType& out) const;
	void fill(const inner::InnerCopyEnter& enter, inner::InnerCopyWay& out) const;
	bool tick();
public:
	void setListIt(TeamVoteListItT it) { listIt = it; }
	void setLastingTime(uint32 lastingTime) { _lastingTime = lastingTime; }
	void setAgreeByDefault(bool agree) { _agreeByDefault = agree; }
	void setCopyPve(const inner::InnerCopyPve& in);
	void setSceneId(zSceneIdType sceneId);
	void startLoading();
public:
	bool addVoter(TeamUser& user);
	bool addVoter(TeamUser& user, bool right);
	void startVote();
	void stopVote();
	void tryPassVote();
	void expireStopVote();
	void confirmVote();
	void userCheck(zRoleIdType roleId, bool success);			//��̫��������
	void userAgree(TeamUser& user);
	void userOppose(TeamUser& user);
	void broadCast(const uint32 first, const uint32 second, const google::protobuf::Message& in);
	bool foreachVoter(std::function<bool(const VoteUserData&)>&& func);
public:
	zVoteIdType getUid() const { return _uid; }
	uint32 state() const { return _state; }
	uint64 getExpireTime() { return _expireTime; }
	uint32 getConfirmTime() { return _confirmTime; }
	bool isOver() const { return state() == VoteState_Over; }
public:
	TeamVoteListItT listIt;
	VoteCheckHandle checkHandle;
	VoteCheckFailHandle checkFailHandle;
	VoteBeginHandle beginHandle;								//ͶƱ��ʼ�ص�(֪ͨͶƱ��ʼ)
	VoteUpdateHandle updateHandle;								//ͶƱ�б仯�ص�
	VotePassHandle passHandle;									//ͶƱͨ���ص�
	VoteRejectHandle rejectHandle;								//ͶƱδͨ���ص�
private:
	void __userVote(TeamUser& user, uint32 method);				//ÿ�����ͶƱ��������Ƿ�ͨ��
	void __voteOver(uint32 result);
private:
	uint32 _state = VoteState_None;
	uint32 _result = VoteResult_None;
	zVoteIdType _uid = 0;
	uint64 _startTime = 0;
	uint32 _expireTime = 0;
	uint64 _confirmTime = 0;
	std::list<VoteUserData> _votes;
	uint32 _lastingTime = 0;
	bool _agreeByDefault = false;								//��ʱĬ��ͬ��
	zSceneIdType _sceneId = 0;
	inner::InnerCopyPve* _pve = nullptr;
};