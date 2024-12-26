#pragma once

#include "CNpc.h"
//≥°æ∞π÷ŒÔNPC
class CNpcMonster final : public CNpc
{
public:
	CNpcMonster(uint32 uiUid, CNpcMgr& oMgr);
	virtual ~CNpcMonster();
public:
	virtual bool Init();
	virtual void Final();
	virtual uint32 GetNpcType() const;
	virtual uint32 GetTid() const;
public:
	bool IsBoss() const;
private:
	//CCmptNpcMonsterAI* m_pCmptAI = nullptr;
};
