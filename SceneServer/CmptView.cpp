#include "zPos.h"
#include "SceneScene.h"
#include "CmptView.h"
#include "SceneMap.h"

CmptView::CmptView() :
	_curPos(),
	_srcPos(),
	_dstPos(),
	_terPos()
{
}

CmptView::~CmptView()
{
}

void CmptView::final()
{
}

bool CmptView::setPos(const zPos& newPos)
{
	auto* pScene = getScene();
	assert(pScene != nullptr);
	if (zMap::oBigWH.x > newPos.x && zMap::oBigWH.y > newPos.y)
	{
		_curPos = newPos;
		_screenId = pScene->m_pSceneMap->Pos2zPosI(_curPos);
		return true;
	}
	else
		return false;
}

bool CmptView::isMovingWalking()
{
	if (_dstPos.empty()) return false;
	if (!_curPos.smallEqual(_dstPos)) return true;
	return !m_lstPath.empty();
}

bool CmptView::moveTo(const zPos& oDst)
{
	_dstPos = oDst;
	return true;
}

bool CmptView::addScreen(const zPos& newpos, const WORD circledir)
{
	auto* scene = getScene();
	assert(scene != nullptr);
	if (!scene->)
}

bool CmptView::freshScreen(const zPos& newpos)
{
}

bool CmptView::removeScreen()
{
}

bool CmptView::changeScreen(const zPosI old)
{
}

void CmptView::sendMeToView()
{
}

void CmptView::sendMeToViewExcept()
{
}

void CmptView::sendMeToViewDirect(const int direct)
{
}

void CmptView::removeMeFromView(const zPosI old)
{
}

void CmptView::removeMeFromViewExcept(const zPosI old)
{
}

void CmptView::removeMeFromViewRDir(const zPosI old, const int direct)
{
}

void CmptView::sendChgFieldToView(std::initializer_list<uint32> changes, bool isfight = false)
{
}

uint32 CmptView::sizeScreenRole()
{
}

uint32 CmptView::sizeScreenMonster()
{
}

uint32 CmptView::sizeNineRole()
{
}

uint32 CmptView::sizeNineMonster()
{
}

void CmptView::serverResetMove()
{
}

void CmptView::setStateId(const uint32 stateId)
{
	_stateId = stateId;
	_stateTimeBegin = GetCurrTime();
}