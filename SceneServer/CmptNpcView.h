#pragma once
#include "CmptView.h"

//NPC视野组件类
class SceneNpc;
class SceneMap;
class CmptNpcView : public CmptView
{
public:
	CmptNpcView(SceneNpc& oNpc);
	virtual ~CmptNpcView();
	virtual bool init();
	virtual void final();
	virtual uint64 getViewId();		//角色唯一id/动态npc唯一id
	virtual WORD getViewType();
	virtual uint64 getSceneId();		//场景id
	virtual uint32 getMapId();		//地图id
	virtual SceneScene* getScene();
	virtual SceneMap* getMap();
	virtual uint32 getSpeed() { return 10000; }
public:
	SceneNpc* npcPtr() { return &_npc; }
	SceneNpc& npcRef() { return _npc; }
private:
	SceneNpc& _npc;
};
