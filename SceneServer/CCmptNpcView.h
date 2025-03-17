#pragma once
#include "CCmptView.h"

//NPC��Ұ�����
class CNpc;
class CCmptNpcView : public CCmptView
{
public:
	CCmptNpcView(CNpc& oNpc);
	virtual ~CCmptNpcView();
	virtual bool Init();
	virtual void Final();
	virtual uint64 GetViewId();		//��ɫΨһid/��̬npcΨһid
	virtual WORD GetViewType();
	virtual uint64 GetSceneId();		//����id
	virtual uint32 GetMapID();		//��ͼid
	virtual uint32 GetSpeed() { return 10000; }
	virtual CSceneScene* GetScene();

public:
	CNpc *NpcPtr() { return &m_oNpc; }
	CNpc &NpcRef() { return m_oNpc; }
private:
	CNpc& m_oNpc;
};
