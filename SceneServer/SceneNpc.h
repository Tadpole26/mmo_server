#pragma once
#include "sdk.h"

//必须通过状态控制(为了让新进入视野内的人看到动画)
enum
{
	NpcState_None = 0,
	NpcState_Born = 1,						//出生
	NpcState_Alive = 2,
	NpcState_Dead = 3,
	NpcState_Leave = 4,						//死亡离场
	NpcState_Fall = 5,						//倒下(大型boss可能倒下状态播放动画)
	NpcState_Body = 6,						//尸体躺下
	NpcState_Hatch = 7,						//孵化
	NpcState_Hidden = 8,					//死亡不释放,隐藏防止重复创建对象
	NpcState_Reborn = 9,
	NpcState_Delegate = 10,					//委任,一些怪物重生,需要怪物组全部死亡,才可以重生
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
	virtual uint32 bodyInterval() { return 0; }								//尸体在地上持续时间
	virtual uint32 fallInterval() { return 0; }								//被击倒动画持续时间
	virtual uint32 leaveInterval() { return 0; }							//离开持续时间
	virtual CmptAI* aiCmptPtr() { return nullptr; }
	virtual CmptFight* fightCmptPtr() { return nullptr; }
	virtual CmptMaster* masterCmptPtr() { return nullptr; }
	virtual CmptSlave* slaveCmptPtr() { return nullptr; }
	virtual CmptProp* propCmptPtr() { return nullptr; }
public:
	virtual bool isNpcA() const { return false; }							//是否召唤物
	virtual bool isNpcB() const { return false; }							//是否阻挡物
	virtual bool isNpcC() const { return false; }							//是否采集物
	virtual bool isNpcF() const { return false; }							//固定npc
	virtual bool isNpcM() const { return false; }							//怪物
	virtual bool isNpcR() const { return false; }							//区域占领
	virtual bool isNpcT() const { return false; }							//触发器
	virtual SceneNpcA* toNpcA() { return nullptr; }
	virtual SceneNpcB* toNpcB() { return nullptr; }
	virtual SceneNpcC* toNpcC() { return nullptr; }
	virtual SceneNpcF* toNpcF() { return nullptr; }
	virtual SceneNpcM* toNpcM() { return nullptr; }
	virtual SceneNpcR* toNpcR() { return nullptr; }
	virtual SceneNpcT* toNpcT() { return nullptr; }
public:
	void bornInitially();													//首次出生
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
	CmptNpcView* _viewcmpt = nullptr;			//视野模块
};