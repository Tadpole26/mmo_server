#include "zPos.h"
#include "CSceneScene.h"
#include "CCmptView.h"
#include "CSceneMap.h"

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

bool CCmptView::SetPos(const zPos& newPos)
{
	auto* pScene = GetScene();
	assert(pScene != nullptr);
	if (zMap::oBigWH.x > newPos.x && zMap::oBigWH.y > newPos.y)
	{
		m_oCurPos = newPos;
		m_uiScreenId = pScene->m_pSceneMap->Pos2zPosI(m_oCurPos);
		return true;
	}
	else
		return false;
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