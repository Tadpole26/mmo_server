#pragma once

#include "CCmptNpcView.h"
//����NPC��Ұ�����
class CNpcMonster;
class CCmptNpcMonsterView final : public CCmptNpcView
{
public:
	CCmptNpcMonsterView(CNpcMonster &oMonster);
	~CCmptNpcMonsterView();
public:
	virtual bool Init();
	virtual void Final();
	virtual uint64 GetSceneId() { return 44444; }
};
