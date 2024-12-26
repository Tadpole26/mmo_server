#include "sdk.h"
#include "CNpc.h"
#include "CCmptNpcView.h"

CNpc::CNpc(uint32 uiNpcId, CNpcMgr& oNpcMgr, CCmptNpcView* pViewCmpt) :
	m_uiUid(uiNpcId),
	m_oNpcMgr(oNpcMgr),
	m_pViewCmpt(pViewCmpt)
{
}

CNpc::~CNpc()
{
}

bool CNpc::Init()
{
	assert(m_pViewCmpt);
	if (!m_pViewCmpt->Init()) return false;
	return true;
}

void CNpc::Final()
{
}