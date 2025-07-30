#pragma once
#include "CmptView.h"

//NPC��Ұ�����
class SceneNpc;
class SceneMap;
class CmptNpcView : public CmptView
{
public:
	CmptNpcView(SceneNpc& oNpc);
	virtual ~CmptNpcView();
	virtual bool init();
	virtual void final();
	virtual uint64 getViewId();		//��ɫΨһid/��̬npcΨһid
	virtual WORD getViewType();
	virtual uint64 getSceneId();		//����id
	virtual uint32 getMapId();		//��ͼid
	virtual SceneScene* getScene();
	virtual SceneMap* getMap();
	virtual uint32 getSpeed() { return 10000; }
public:
	SceneNpc* npcPtr() { return &_npc; }
	SceneNpc& npcRef() { return _npc; }
private:
	SceneNpc& _npc;
};
