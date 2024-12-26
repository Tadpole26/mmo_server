#pragma once

#include "CCmptNpcView.h"
//怪物NPC视野组件类
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
