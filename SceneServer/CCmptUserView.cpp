#include "CCmptUserView.h"
#include "CGameUser.h"

CCmptUserView::CCmptUserView(CGameUser* pUser) : CCmptView()
{
	this->m_pUser = pUser;
}

CCmptUserView::~CCmptUserView()
{
}

void CCmptUserView::Final()
{
	CCmptView::Final();
}

bool CCmptUserView::Init()
{
	return true;
}

uint64 CCmptUserView::GetViewId()
{
	return m_pUser->GetRoleId();
}

uint16 CCmptUserView::GetViewType()
{
	return ROLE_TYPE;
}

uint64 CCmptUserView::GetSceneId()
{
}

uint32 CCmptUserView::GetMapID()
{
}

uint32 CCmptUserView::GetSpeed()
{
}