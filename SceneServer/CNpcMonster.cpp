#include "zType.h"
#include "CNpcMonster.h"

#include "CCmptNpcView.h"

CNpcMonster::CNpcMonster(uint32 uiUid, CNpcMgr& oMgr) :
	CNpc(uiUid, oMgr, new CCmptNpcView(*this))

{
}

CNpcMonster::~CNpcMonster()
{
}

bool CNpcMonster::Init()
{
	if (!CNpc::Init())
		return false;
	return true;
}

void CNpcMonster::Final()
{
}

uint32 CNpcMonster::GetNpcType() const
{
	return 0;
}

uint32 CNpcMonster::GetTid() const
{
	return 0;
}

bool CNpcMonster::IsBoss() const
{
	return false;
}