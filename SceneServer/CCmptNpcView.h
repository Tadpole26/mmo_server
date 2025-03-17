#pragma once
#include "CCmptView.h"

//NPC视野组件类
class CNpc;
class CCmptNpcView : public CCmptView
{
public:
	CCmptNpcView(CNpc& oNpc);
	virtual ~CCmptNpcView();
	virtual bool Init();
	virtual void Final();
	virtual uint64 GetViewId();		//角色唯一id/动态npc唯一id
	virtual WORD GetViewType();
	virtual uint64 GetSceneId();		//场景id
	virtual uint32 GetMapID();		//地图id
	virtual uint32 GetSpeed() { return 10000; }
	virtual CSceneScene* GetScene();

public:
	CNpc *NpcPtr() { return &m_oNpc; }
	CNpc &NpcRef() { return m_oNpc; }
private:
	CNpc& m_oNpc;
};
