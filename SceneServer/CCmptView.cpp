#include "zPos.h"
#include "CCmptView.h"

CCmptView::CCmptView() :
	m_oCurPos(),
	m_oSrcPos(),
	m_oDstPos(),
	m_oTerPos()
{
}

CCmptView::~CCmptView()
{
}

void CCmptView::Final()
{
}

bool CCmptView::IsMovingWalking()
{
	if (m_oDstPos.empty()) return false;
	if (!m_oCurPos.smallEqual(m_oDstPos)) return true;
	return !m_lstPath.empty();
}

bool CCmptView::MoveTo(const zPos& oDst)
{
	m_oDstPos = oDst;
	return true;
}