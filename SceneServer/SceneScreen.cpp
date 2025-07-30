#include "SceneScreen.h"

#include "CmptView.h"
#include "CCmptNpcView.h"
#include "SceneMap.h"
#include "CNpc.h"

const zPosIVector& SceneScreen::getFullScreen()
{
	static zPosIVector kFullScreen{ SceneMap::kFullScreenIndex };
	return kFullScreen;
}

const zPosIVector& SceneScreen::getEmptyScreen()
{
	static zPosIVector kEmptyScreen;
	return kEmptyScreen;
}

SceneScreen::SceneScreen() :
	zScreenIndex()
{
	for (uint32 type = eViewType_Role; type < eViewType_Max; ++type)
	{
		_viewcount[type] = 0;
		_indexvec[type].clear();
		_indexset[type].clear();
		_allset[type].clear();
	}
}

SceneScreen::~SceneScreen()
{
}

void SceneScreen::__addScreenRole(const zPosI uiScreen, CmptView* pView)
{
	if (pView->getViewType() == eViewType_Role)
		++_rolecount[uiScreen];
}

void SceneScreen::__subScreenRole(const zPosI uiScreen, CmptView* pView)
{
	if (pView->getViewType() == eViewType_Role)
		_rolecount[uiScreen]--;
}

void SceneScreen::__addScreenMonster(const zPosI uiScreen, CmptView* pView)
{
	if (pView->getViewType() == eViewType_Npc)
	{
		auto* npcView = static_cast<CCmptNpcView*>(pView);
		if (npcView->NpcRef().IsNpcM())
			++_monstercount[uiScreen];
	}
}

void SceneScreen::__subScreenMosnter(const zPosI uiScreen, CmptView* pView)
{
	if (pView->getViewType() == eViewType_Npc)
	{
		auto* npcView = static_cast<CCmptNpcView*>(pView);
		if (npcView->NpcRef().IsNpcM())
			_monstercount[uiScreen]--;
	}
}

const zPosIVector& SceneScreen::__getNineScreen(const zPosI& uiPosI)
{
	const auto* pVecScreen = GetNineScreen(uiPosI);
	if (!pVecScreen || map->isFullScreen())
		return getFullScreen();
	else
		return *pVecScreen;
}

void SceneScreen::__getScreenByRange(const zPos& oPos, const int iRange, zPosIVector& vecPosI)
{
	int ox = oPos.x % SCREEN_GRID_WIDTH;
	int oy = oPos.y % SCREEN_GRID_HEIGHT;
	int nox = SCREEN_GRID_WIDTH - ox;
	int noy = SCREEN_GRID_HEIGHT - oy;
	int x = oPos.x / SCREEN_GRID_WIDTH;
	int y = oPos.y / SCREEN_GRID_HEIGHT;

	vecPosI.push_back(y*(int)zMap::screenX + x);
	if (iRange > oy)
	{
		if (y > 0)
		{
			vecPosI.push_back((y-1)*(int)zMap::screenX + x);
			if (x < (int)zMap::screenX && iRange > nox)
			{
				int dis = (int)sqrtf((float)(oy * oy) + float(nox * nox));
				if (iRange > dis)
				{
					vecPosI.push_back((y-1)*(int)zMap::screenX + x + 1);
				}
			}
		}
	}
	if (iRange > nox)
	{
		if (x < (int)zMap::screenX)
		{
			vecPosI.push_back(y*(int)zMap::screenX + x + 1);
			if (y < (int)zMap::screenY && iRange > noy)
			{
				int dis = (int)sqrtf((float)(nox * nox) + (float)(noy * noy));
				if (iRange > dis)
				{
					vecPosI.push_back((y + 1)*(int)zMap::screenX + x + 1);
				}
			}
		}
	}
	if (iRange > noy)
	{
		if (y < (int)zMap::screenX)
		{
			vecPosI.push_back((y + 1) * (int)zMap::screenX + x);
			if (x > 0 && iRange > ox)
			{
				int dis = (int)sqrtf((float)(noy * noy) + (float)(ox * ox));
				if (iRange < dis)
				{
					vecPosI.push_back((y + 1) * (int)zMap::screenX + x - 1);
				}
			}
		}
	}
	if (iRange > ox)
	{
		if (x > 0)
		{
			vecPosI.push_back(y * (int)zMap::screenX + x - 1);
			if (y > 0 && iRange > oy)
			{
				int dis = (int)sqrtf((float)(ox * ox) + (float)(oy * oy));
				if (iRange > dis)
				{
					vecPosI.push_back((y - 1) * (int)zMap::screenX + x - 1);
				}
			}
		}
	}
}

void SceneScreen::__getScreenByArea(const zPos& oLeftdown, const zPos& oRightup, zPosIVector& vecPosI)
{
	int leftx = oLeftdown.x / SCREEN_GRID_WIDTH;
	int lefty = oLeftdown.y / SCREEN_GRID_HEIGHT;
	int rightx = oRightup.x / SCREEN_GRID_WIDTH;
	int righty = oRightup.y / SCREEN_GRID_HEIGHT;

	for (auto x = leftx; x <= rightx; ++x)
	{
		for (auto y = lefty; y <= righty; ++y)
		{
			vecPosI.push_back(y * (int)zMap::screenX + x);
		}
	}
}

uint32 SceneScreen::sizeNineRole(const zPosI uiScreen)
{
	uint32 uiCount = 0;
	const auto& vecScreen = __getNineScreen(uiScreen);
	for (auto it = vecScreen.begin(); it != vecScreen.end(); ++it)
		uiCount += _rolecount[*it];
	return uiCount;
}

uint32 SceneScreen::sizeNineMonster(const zPosI uiScreen)
{
	uint32 uiCount = 0;
	const auto& vecScreen = __getNineScreen(uiScreen);
	for (auto it = vecScreen.begin(); it != vecScreen.end(); ++it)
		uiCount += _monstercount[*it];
	return uiCount;
}

bool SceneScreen::addScreen(CmptView* pView, const zPos& oNewPos)
{
	if (pView->hasInScene()) return false;

	auto type = pView->getViewType();
	if (pView->setPos(oNewPos))
	{
		zPosI uiScreen = pView->getPosI();
		if (_indexset[type][uiScreen].insert(pView).second)
		{
			_indexvec[type][uiScreen].push_back(pView);
		}
		++_viewcount[type];
		_allset[type].insert(pView);
		pView->setInScene();
		__addScreenRole(uiScreen, pView);
		__addScreenMonster(uiScreen, pView);
	}
	return pView->hasInScene();
}

bool SceneScreen::freshScreen(CmptView* pView, const zPos& oNewPos)
{
	if (pView->hasInScene()) return false;
	
	bool bRet = false;
	auto type = pView->getViewType();
	zPosI uiOld = pView->getPosI();
	CmptViewSetT& setViews = _indexset[type][uiOld];
	CmptViewVecT& vecViews = _indexvec[type][uiOld];
	CmptViewSetT::const_iterator iter = setViews.find(pView);
	if (iter != setViews.end() && pView->setPos(oNewPos))
	{
		bRet = true;
		zPosI uiNew = pView->getPosI();
		if (uiOld != uiNew)
		{
			setViews.erase(iter);
			SimpleRemoveFromVec(vecViews, pView);
			if (_indexset[type][uiNew].insert(pView).second)
				_indexvec[type][uiNew].push_back(pView);
			__subScreenRole(uiOld, pView);
			__subScreenMosnter(uiOld, pView);
			__addScreenRole(uiNew, pView);
			__addScreenMonster(uiNew, pView);
			pView->notifyGatewayFreshScreen();
		}
	}
	return bRet;
}

bool SceneScreen::removeScreen(CmptView* pView)
{
	if (pView->hasInScene()) return false;

	auto type = pView->getViewType();
	zPosI uiScreen = pView->getPosI();
	CmptViewSetT& setViews = _indexset[type][uiScreen];
	CmptViewVecT& vecViews = _indexvec[type][uiScreen];
	CmptViewSetT::iterator iter = setViews.find(pView);
	if (iter != setViews.end())
	{
		setViews.erase(iter);
		SimpleRemoveFromVec(vecViews, pView);
		_allset[type].erase(pView);
		_viewcount[type]--;
		__subScreenRole(uiScreen, pView);
		pView->setOutScene();
		return true;
	}
	return false;
}

void SceneScreen::execAllOfScene(const uint32 uiType, CmptViewCallback& callback)
{
	for (auto it = _allset[uiType].begin(); it != _allset[uiType].end();)
	{
		auto tmp = it++;
		auto* pView = *tmp;
		if (!callback.exec(pView))
			return;
	}
}

bool SceneScreen::execAllOfScene(const uint32 uiType, CmptViewFuncT&& func)
{
	return std::all_of(_allset[uiType].begin(), _allset[uiType].end(), [&](auto* view) { return func(*view); });
}

void SceneScreen::execAllOfScene(CmptViewCallback& callback)
{
	for (uint32 type = eViewType_Role; type < eViewType_Max; ++type)
		execAllOfScene(type, callback);
}

void SceneScreen::execAllOfScene(CmptViewFuncT&& func)
{
	for (uint32 type = eViewType_Role; type < eViewType_Max; ++type)
		execAllOfScene(type, CmptViewFuncT(func));
}

void SceneScreen::execAllOfScreen(const uint32 uiType, const zPosI uiScreen, CmptViewCallback& callback)
{
	CmptViewVecT& vecViews = _indexvec[uiType][uiScreen];
	for (size_t i = 0; i < vecViews.size(); i++)
	{
		if (!callback.exec(vecViews[i]))
			return;
	}
}

void SceneScreen::execAllOfScreen(const uint32 uiType, const zPosI uiScreen, CmptViewFuncT&& func)
{
	CmptViewVecT& vecViews = _indexvec[uiType][uiScreen];
	for (size_t i = 0; i < vecViews.size(); i++)
	{
		if (!func(*vecViews[i]))
			return;
	}
}

void SceneScreen::execAllOfScreen(const zPosI uiScreen, CmptViewCallback& callback)
{
	for (uint32 type = eViewType_Role; type < eViewType_Max; ++type)
	{
		CmptViewVecT& vecViews = _indexvec[type][uiScreen];
		for (size_t i = 0; i < vecViews.size(); i++)
		{
			if (!callback.exec(vecViews[i]))
				return;
		}
	}
}

void SceneScreen::execAllOfScreen(const zPosI uiScreen, CmptViewFuncT&& func)
{
	for (uint32 type = eViewType_Role; type < eViewType_Max; ++type)
	{
		CmptViewVecT& vecViews = _indexvec[type][uiScreen];
		for (size_t i = 0; i < vecViews.size(); i++)
		{
			if (!func(*vecViews[i]))
				return;
		}
	}
}

void SceneScreen::execAllOfNine(const uint32 uiType, const zPosI uiScreen, CmptViewCallback& callback)
{
	const auto& vecScreens = __getNineScreen(uiScreen);
	for (auto it = vecScreens.begin(); it != vecScreens.end(); ++it)
	{
		execAllOfScreen(uiType, *it, callback);
	}
}

void SceneScreen::execAllOfNine(const uint32 uiType, const zPosI uiScreen, CmptViewFuncT&& func)
{
	const auto& vecScreens = __getNineScreen(uiScreen);
	std::for_each(vecScreens.begin(), vecScreens.end(), [&](auto posI) { ExecAllOfScreen(uiType, posI, std::move(func)); });
}

void SceneScreen::execAllOfNine(const zPosI uiScreen, CmptViewCallback& callback)
{
	const auto& vecScreens = __getNineScreen(uiScreen);
	for (auto it = vecScreens.begin(); it != vecScreens.end(); ++it)
	{
		execAllOfScreen(*it, callback);
	}
}

void SceneScreen::execAllOfNine(const zPosI uiScreen, CmptViewFuncT&& func)
{
	const auto& vecScreens = __getNineScreen(uiScreen);
	std::for_each(vecScreens.begin(), vecScreens.end(), [&](auto posI) { ExecAllOfScreen(posI, std::move(func)); });
}

void SceneScreen::execAllOfDirect(const uint32 uiType, const zPosI uiScreen, const int direct, CmptViewCallback& callback)
{
	const auto* pVecScreens = GetDirectScreen(uiScreen, direct);
	if (map->isFullScreen() || !pVecScreens) return;
	for (auto it = pVecScreens->begin(); it != pVecScreens->end();++it)
	{
		execAllOfScreen(uiType, *it, callback);
	}
}

void SceneScreen::execAllOfDirect(const zPosI uiScreen, const int direct, CmptViewCallback& callback)
{
	const auto* pVecScreens = GetDirectScreen(uiScreen, direct);
	if (map->isFullScreen() || !pVecScreens) return;
	for (auto it = pVecScreens->begin(); it != pVecScreens->end(); ++it)
	{
		execAllOfScreen(*it, callback);
	}
}

void SceneScreen::execAllOfRDirect(const uint32 uiType, const zPosI uiScreen, const int direct, CmptViewCallback& callback)
{
	const auto* pVecScreens = GetReverseDirectScreen(uiScreen, direct);
	if (map->isFullScreen() || !pVecScreens) return;
	for (auto it = pVecScreens->begin(); it != pVecScreens->end(); ++it)
	{
		execAllOfScreen(uiType, *it, callback);
	}
}

void SceneScreen::execAllOfRDirect(const zPosI uiScreen, const int direct, CmptViewCallback& callback)
{
	const auto* pVecScreens = GetReverseDirectScreen(uiScreen, direct);
	if (map->isFullScreen() || !pVecScreens) return;
	for (auto it = pVecScreens->begin(); it != pVecScreens->end(); ++it)
	{
		execAllOfScreen(*it, callback);
	}
}

void SceneScreen::execAllOfPosRange(const zPos& oPos, const int iRange, CmptViewCallback& callback)
{
	zPosIVector vecScreen;
	if (map->isFullScreen()) vecScreen = getFullScreen();
	else __getScreenByRange(oPos, iRange, vecScreen);
	for (zPosIVector::const_iterator it = vecScreen.begin(); it != vecScreen.end(); ++it)
	{
		execAllOfScreen(*it, callback);
	}
}

void SceneScreen::execAllOfPosRange(const uint32 uiType, const zPos& oPos, const int iRange, CmptViewCallback& callback)
{
	zPosIVector vecScreen;
	if (map->isFullScreen()) vecScreen = getFullScreen();
	else __getScreenByRange(oPos, iRange, vecScreen);
	for (zPosIVector::const_iterator it = vecScreen.begin(); it != vecScreen.end(); ++it)
	{
		execAllOfScreen(uiType, *it, callback);
	}
}

void SceneScreen::execAllOfPosArea(const zPos& oLeftdown, const zPos& oRightup, CmptViewCallback& callback)
{
	zPosIVector vecScreen;
	if (map->isFullScreen()) vecScreen = getFullScreen();
	else __getScreenByArea(oLeftdown, oRightup, vecScreen);
	for (zPosIVector::const_iterator it = vecScreen.begin(); it != vecScreen.end(); ++it)
	{
		execAllOfScreen(*it, callback);
	}
}

void SceneScreen::execAllOfPosArea(const uint32 uiType, const zPos& oLeftdown, const zPos& oRightup, CmptViewCallback& callback)
{
	zPosIVector vecScreen;
	if (map->isFullScreen()) vecScreen = getFullScreen();
	else __getScreenByArea(oLeftdown, oRightup, vecScreen);
	for (zPosIVector::const_iterator it = vecScreen.begin(); it != vecScreen.end(); ++it)
	{
		execAllOfScreen(uiType, *it, callback);
	}
}