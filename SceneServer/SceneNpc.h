#pragma once
#include "sdk.h"

//����ͨ��״̬����(Ϊ�����½�����Ұ�ڵ��˿�������)
enum
{
	NpcState_None = 0,
	NpcState_Born = 1,						//����
	NpcState_Alive = 2,
	NpcState_Dead = 3,
	NpcState_Leave = 4,						//�����볡
	NpcState_Fall = 5,						//����(����boss���ܵ���״̬���Ŷ���)
	NpcState_Body = 6,						//ʬ������
	NpcState_Hatch = 7,						//����
	NpcState_Hidden = 8,					//�������ͷ�,���ط�ֹ�ظ���������
	NpcState_Reborn = 9,
	NpcState_Delegate = 10,					//ί��,һЩ��������,��Ҫ������ȫ������,�ſ�������
	NpcState_Destroy = 11,
};

class SceneNpcMgr;
class CmptNpcView;
class SceneScene;
class CmptAI;
class CmptFight;
class CmptMaster;
class CmptSlave;
class CmptProp;
class SceneNpcA;
class SceneNpcB;
class SceneNpcC;
class SceneNpcF;
class SceneNpcM;
class SceneNpcP;
class SceneNpcR;
class SceneNpcT;
class SceneNpc
{
public:
	SceneNpc(uint32 npcuid, SceneNpcMgr& owner, CmptNpcView *viewcmpt);
	virtual ~SceneNpc();
public:
	virtual bool init();
	virtual void final();
	virtual uint32 getNpcType() const = 0;
	virtual uint32 getTid() const = 0;
	virtual void onDead() {}
	virtual void onAppear() {}
	virtual void onDisappear() {}
	virtual void onHatch() {}
	virtual bool canBeDestroied() { return true; }
	virtual uint32 bornInterval() { return 0; }
	virtual uint32 rebornInterval() { return 1000; }
	virtual uint32 bodyInterval() { return 0; }								//ʬ���ڵ��ϳ���ʱ��
	virtual uint32 fallInterval() { return 0; }								//��������������ʱ��
	virtual uint32 leaveInterval() { return 0; }							//�뿪����ʱ��
	virtual CmptAI* aiCmptPtr() { return nullptr; }
	virtual CmptFight* fightCmptPtr() { return nullptr; }
	virtual CmptMaster* masterCmptPtr() { return nullptr; }
	virtual CmptSlave* slaveCmptPtr() { return nullptr; }
	virtual CmptProp* propCmptPtr() { return nullptr; }
public:
	virtual bool isNpcA() const { return false; }							//�Ƿ��ٻ���
	virtual bool isNpcB() const { return false; }							//�Ƿ��赲��
	virtual bool isNpcC() const { return false; }							//�Ƿ�ɼ���
	virtual bool isNpcF() const { return false; }							//�̶�npc
	virtual bool isNpcM() const { return false; }							//����
	virtual bool isNpcR() const { return false; }							//����ռ��
	virtual bool isNpcT() const { return false; }							//������
	virtual SceneNpcA* toNpcA() { return nullptr; }
	virtual SceneNpcB* toNpcB() { return nullptr; }
	virtual SceneNpcC* toNpcC() { return nullptr; }
	virtual SceneNpcF* toNpcF() { return nullptr; }
	virtual SceneNpcM* toNpcM() { return nullptr; }
	virtual SceneNpcR* toNpcR() { return nullptr; }
	virtual SceneNpcT* toNpcT() { return nullptr; }
public:
	void bornInitially();													//�״γ���
	void bornDynamically();
	void bornByHatch();
	void die();
	void toDeath();
	void toLeave();
	void toFall();
	void toBody();
	void toBorn();
	void toHatch();
	void toAlive();
	void toHidden();
	void toReborn();
	void toDelegate();
	void toDestroy();
	bool isBorn() const { return _state == NpcState_Born; }
	bool isAlive() const { return _state == NpcState_Alive; }
	bool isDead() const { return _state == NpcState_Dead; }
	bool isLeave() const { return _state == NpcState_Leave; }
	bool isFall() const { return _state == NpcState_Fall; }
	bool isBody() const { return _state == NpcState_Body; }
	bool isHatch() const { return _state == NpcState_Hatch; }
	bool isHidden() const { return _state == NpcState_Hidden; }
	bool isReborn() const { return _state == NpcState_Reborn; }
	bool isDelegate() const { return _state == NpcState_Delegate; }
	bool isDestroy() const { return _state == NpcState_Destroy; }
	bool isAfterHidden() const { return _state > NpcState_Hidden; }
public:
	CmptNpcView* viewCmptPtr() { return _viewcmpt; }
	CmptNpcView& viewCmptRef() { return *_viewcmpt; }
public:
	zRoleIdType getUid() const { return _uid; }
	uint32 getState() const { return _state; }
	uint32 getRebornTimes() const { return _rebornTimes; }
	void setRebornTimes(uint32 rebornTimes) { _rebornTimes = rebornTimes; }
	uint64 getStateTimeBegin() const { return _stateTimeBegin; }
	uint64 getStateTimeEnd() const { return _stateTimeEnd; }
public:
	SceneScene* scene = nullptr;
private:
	zRoleIdType _uid = 0;
	uint32 _state = NpcState_None;
	uint64 _stateTimeBegin = 0;
	uint64 _stateTimeEnd = 0;
	uint32 _rebornTimes = 0;
	SceneNpcMgr& _owner;
	CmptNpcView* _viewcmpt = nullptr;			//��Ұģ��
};