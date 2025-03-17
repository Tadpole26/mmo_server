#include "zType.h"
#include "CNpc.h"
#include "CCmptNpcView.h"

CCmptNpcView::CCmptNpcView(CNpc& oNpc) :
	CCmptView(),
	m_oNpc(oNpc)
{
}

CCmptNpcView::~CCmptNpcView()
{
}

bool CCmptNpcView::Init()
{
	return true;
}

void CCmptNpcView::Final()
{
	CCmptView::Final();
}

uint64 CCmptNpcView::GetViewId()
{
	return NpcRef().GetUid();
}

WORD CCmptNpcView::GetViewType()
{
	return NpcRef().GetNpcType();
}

uint64 CCmptNpcView::GetSceneId()
{
	return 0;
}

uint32 CCmptNpcView::GetMapID()
{
	return 0;
}

CSceneScene* CCmptNpcView::GetScene()
{
	return nullptr;
}