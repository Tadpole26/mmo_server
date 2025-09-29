#pragma once

#include "cScene.h"

class CmptView;
class SceneMap;
class SceneScreen;
class GameUser;
class SceneNpc;
class SceneNpcFMgr;
class SceneNpcM;
class SceneNpcMMgr;
class SceneNpcC;
class SceneNpcCMgr;
class SceneNpcR;
class SceneNpcRMgr;
class SceneNpcP;
class SceneNpcPMgr;
class CopyScene;
class CopyCmptLevel;
class CopyCmptEvent;
class CopyCmptState;
class CopyCmptFunc;
class SceneScene : public cScene
{
public:
	SceneScene(const uint64 ullSceneId);
	virtual ~SceneScene();
	void final() override;
public:
	virtual bool init();
	virtual void tick();
	virtual bool isInitMonster() { return true; }
	virtual bool isInitCollect() { return true; }
	virtual bool intoPosition(GameUser* pUser, zPos& inout);					//随机进入场景
	virtual bool rebirthPosition(GameUser* pUser, zPos& inout);					//随机重生场景
	virtual uint32 getRebirthDirect();											//获取重生方向
	virtual void onRoleEnter(GameUser* pUser);									//玩家进入场景
	virtual void onRoleLeave(GameUser* pUser);									//玩家离开场景
	virtual void onNpcEnter(SceneNpc* pNpc);									//npc进入场景
	virtual void onNpcLeave(SceneNpc* pNpc);									//npc离开场景
public:
	//inline SceneScreen& getScreen() { return *_sceneScreen; }
	inline SceneNpcFMgr& npcfMgr() { return *_npcfmgr; }
	inline SceneNpcMMgr& npcmMgr() { return *_npcmmgr; }
public:
	//副本
	virtual CopyScene* toCopyScene() { return nullptr; }
	virtual CopyCmptLevel* cmptLevelPtr() { return nullptr; }
	virtual CopyCmptEvent* cmptEventPtr() { return nullptr; }
	virtual CopyCmptState* cmptStatePtr() { return nullptr; }
public:
	//bool broadCmdToScene(const WORD first, const WORD second, const google::protobuf::Message& in);
	//bool broadCmdToScene(const void* pstrCmd, const DWORD size);
	//bool broadStaticViewToScene();
public:
	virtual void eventAcceptTask(uint32 taskId) {}
	virtual void eventFinishTask(uint32 taskId) {}
	virtual void eventMonsterHurt(SceneNpcM& npcm, CmptView& from, uint32 val) {}
	virtual void eventMonsterKilled(SceneNpcM& npcm, CmptView& from) {}
	virtual void eventRoleKilled(GameUser& killed, CmptView* killer) {}
	virtual void eventMonsterActivityChange(SceneNpcM& npcm) {}
	virtual void eventCollected(SceneNpcC& npcc, CmptView& from) {}
	virtual void eventRegionalUpdate(SceneNpcR& npcr, uint32 campId, uint32 num) {}
	virtual void eventPuzzleEnd(SceneNpcP& npcp) {}
public:
	void eventWorldLevelChange(uint32 worldlevel);
	void eventNpcMGroupCleared(uint32 npcMGroupId);
	void eventNpcMGroupReborn(uint32 npcMGroupId);
	void eventMvpStateChange();
	void eventMiniBossOpen(uint32 bossId);
	void eventMiniBossClose(uint32 bossId);
	void eventNpcPGroupCleared(uint32 npcPGroupId);
	void eventNpcPGroupReborn(uint32 npcPGroupId);
public:
	uint32 createMapObjectId();
	void resetMapItemId();
public:
	SceneMap* map = nullptr;
private:
	//SceneScreen* _sceneScreen = nullptr;
	SceneNpcFMgr* _npcfmgr = nullptr;
	SceneNpcMMgr* _npcmmgr = nullptr;
};
