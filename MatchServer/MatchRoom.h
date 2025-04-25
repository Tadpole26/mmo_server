#pragma once

#include "cMatch.h"
#include "MatchUnit.h"

namespace inner
{
	class InnerMatchRoom;
	class InnerMatchRoomCopyScene;
	class InnerCopyCrit;
	class InnerCopyTeamList;
	class InnerCopyWayByMatch;
	class InnerCopyType;
	class InnerCopyCreateBy;
};

//����״̬
enum
{
	eStageCreate,				//����
	eStageWaitConfirm,			//�ȴ�ȷ��
	eStageWaitCopyScene,		//�ȴ����Ǹ���
	eStageCancelRoom,			//ȡ������
	eStageFinishRoom,			//��������
	eStageTimeoutRoom,			//��ʱ����

	eStageMax,					//���ֵ
};

inline const char* gettageString(uint32 stage)
{
	assert(stage < eStageMax);
	static const char* stageStr[] =
	{
	"eStageCreate",				//����
	"eStageWaitConfirm",		//�ȴ�ȷ��
	"eStageWaitCopyScene",		//�ȴ���������
	"eStageCancelRoom",			//��ֹ����
	"eStageFinishRoom",			//��������
	"eStageTimeoutRoom",		//��ʱ����
	};
	return stageStr[stage];
}

//����״̬����
typedef void(MatchRoom::*RoomStageFuncPtr)();

class MatchGroup;
class MatchQueueMgr;
class MatchRoom
{
public:
	using UnitListT = std::list<MatchUnit*>;
	using GroupListT = std::vector<UnitListT>;
	using FuncGroupUnitT = std::function<bool(MatchUnit&)>;
public:
	MatchRoom(zRoomIdType roomId, std::string&& fromstr, MatchQueueMgr& mgr);
	virtual ~MatchRoom();
	virtual void final();
	virtual void addGroup(MatchGroup& group);
public:
	virtual void fill(inner::InnerMatchRoom& out);
	virtual void fill(inner::InnerCopyCrit& out);
	virtual void fill(inner::InnerCopyTeamList& out) = 0;
	virtual void fill(inner::InnerCopyWayByMatch& out);
	virtual void fill(inner::InnerCopyType& out) = 0;
public:
	void tick();
	void removeUnit(MatchUnit* pUnit);
	bool foreachUnit(uint32 groupId, FuncGroupUnitT func);
	MatchQueueMgr& mgrRef() { return _mgr; }
	zRoomIdType getRoomId() { return _roomId; }
	uint32 initMember() { return _initsize; }
	uint32 sizeMember() { return _cursize; }
public:
	bool isStageClientConfirm();
	uint32 getRoomTimeoutExpire();
	bool isTimeForRoomTimeout();
	uint32 getConfirmTimeoutExpire();
	bool isTimeForConfirm();
	bool isTimeForConfirmRobot();
	bool isTimeForConfirmTimeout();
	uint32 getCreateCopyTimeoutExpire();
	bool isTimeForCreateCopy();
	bool isTimeForCreateCopyTimeout();
public:
	void forceBreakNtf();
	void forceBreak();
	void trySetConfirmOk();
	void setConfirmRandRobot();
	void setConfirmRefuse();
	void setConfirmTimeout();
	void trySetCopySceneOk();
	void setCopySceneTimeout();
	void setCreateCopyResult(zSceneIdType sceneId, uint32 hashId, const inner::InnerCopyCreateBy& createby);
protected:
	void __addRed(MatchUnit* pUnit);
	void __addBlue(MatchUnit* pUnit);
	void __tryAddUnitInfo(MatchUnit& unit);
	void __setStage(uint32 stage);
	void __funcCreateRoom();
	void __funcWaitConfirm();
	void __funcWaitCopyScene();
	void __funcCancelRoom();
	void __funcFinishRoom();
	void __funcTimeoutRoom();
protected:
	MatchQueueMgr& _mgr;								//������
	GroupListT _groups;									//��Ԫ��
	zRoomIdType _roomId = 0;							//����Id
	std::string _formStr;								//����
	uint64 _createTime = 0;								//���䴴��ʱ��
	uint32 _initsize = 0;								//��ʼ����
	uint32 _cursize = 0;								//��ǰ����
	uint32 _stage = eStageCreate;						//����stage
	std::list<uint32> _unitZoneList;					//��Ԫ���б�
	inner::InnerMatchRoomCopyScene* _copyscene = nullptr;
	inner::InnerCopyCreateBy* _createby = nullptr;
private:
	RoomStageFuncPtr _funcs[eStageMax] =
	{
		&MatchRoom::__funcCreateRoom,
		&MatchRoom::__funcWaitConfirm,
		&MatchRoom::__funcWaitCopyScene,
		&MatchRoom::__funcCancelRoom,
		&MatchRoom::__funcFinishRoom,
		&MatchRoom::__funcTimeoutRoom
	};
};