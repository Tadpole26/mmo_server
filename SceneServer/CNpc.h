#pragma once

class CNpcMgr;
class CCmptNpcView;
enum
{
	NpcState_None = 0,
	NpcState_Born = 1,
	NpcState_Alive = 2,
	NpcState_Dead = 3,
	NpcState_Leave = 4,
	NpcState_Fall = 5,
	NpcState_Destroy = 6,
};
//场景NPC基类
class CNpc
{
public:
	CNpc(uint32 uiNpcId, CNpcMgr& oNpcMgr, CCmptNpcView *pViewCmpt);
	virtual ~CNpc();
public:
	virtual bool Init();
	virtual void Final();
	virtual uint32 GetNpcType() const = 0;
	virtual uint32 GetTid() const = 0;
public:
	uint32 GetUid() const { return m_uiUid; }
public:
	CCmptNpcView* ViewCmptPtr() { return m_pViewCmpt; }
	CCmptNpcView& ViewCmptRef() { return *m_pViewCmpt; }
public:
	virtual bool IsNpcM() const { return false; }
	virtual bool IsNpcB() const { return false; }
private:
	uint32 m_uiUid = 0;
	CNpcMgr& m_oNpcMgr;			
	CCmptNpcView* m_pViewCmpt = nullptr;			//视野模块
};