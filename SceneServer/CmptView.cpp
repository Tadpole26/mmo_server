//#include "SceneScene.h"
#include "CmptView.h"
//#include "SceneMap.h"

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
	/*
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
		*/
	return true;
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
	//auto* scene = getScene();
	//assert(scene != nullptr);
	//if (!scene->)
	return true;
}

bool CmptView::freshScreen(const zPos& newpos)
{
	return true;
}

bool CmptView::removeScreen()
{
	return true;
}

bool CmptView::changeScreen(const zPosI old)
{
	return true;
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

void CmptView::sendChgFieldToView(std::initializer_list<uint32> changes, bool isfight)
{
}

uint32 CmptView::sizeScreenRole()
{
	return 0;
}

uint32 CmptView::sizeScreenMonster()
{
	return 0;
}

uint32 CmptView::sizeNineRole()
{
	return 0;
}

uint32 CmptView::sizeNineMonster()
{
	return 0;
}

void CmptView::serverResetMove()
{
}

void CmptView::setStateId(const uint32 stateId)
{
	_stateId = stateId;
	_stateTimeBegin = GetCurrTime();
}