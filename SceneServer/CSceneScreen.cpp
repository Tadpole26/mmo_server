#include "CSceneScreen.h"

#include "CCmptView.h"
#include "CCmptNpcView.h"
#include "CSceneMap.h"
#include "CNpc.h"

const zPosIVector& CSceneScreen::GetFullScreen()
{
	static zPosIVector kFullScreen{ CSceneMap::kFullScreenIndex };
	return kFullScreen;
}

const zPosIVector& CSceneScreen::GetEmptyScreen()
{
	static zPosIVector kEmptyScreen;
	return kEmptyScreen;
}

CSceneScreen::CSceneScreen() :
	zScreenIndex()
{
	for (uint32 type = eViewType_Role; type < eViewType_Max; ++type)
	{
		m_arrViewCount[type] = 0;
		m_mapSetIndex[type].clear();
		m_mapVecIndex[type].clear();
		m_setAll[type].clear();
	}
}

CSceneScreen::~CSceneScreen()
{
}

void CSceneScreen::__AddScreenRole(const zPosI uiScreen, CCmptView* pView)
{
	if (pView->GetViewType() == eViewType_Role)
		++m_mapRoleCount[uiScreen];
}

void CSceneScreen::__SubScreenRole(const zPosI uiScreen, CCmptView* pView)
{
	if (pView->GetViewType() == eViewType_Role)
		m_mapRoleCount[uiScreen]--;
}

void CSceneScreen::__AddScreenMonster(const zPosI uiScreen, CCmptView* pView)
{
	if (pView->GetViewType() == eViewType_Npc)
	{
		auto* npcView = static_cast<CCmptNpcView*>(pView);
		if (npcView->NpcRef().IsNpcM())
			++m_mapMonsterCount[uiScreen];
	}
}

void CSceneScreen::__SubScreenMosnter(const zPosI uiScreen, CCmptView* pView)
{
	if (pView->GetViewType() == eViewType_Npc)
	{
		auto* npcView = static_cast<CCmptNpcView*>(pView);
		if (npcView->NpcRef().IsNpcM())
			m_mapMonsterCount[uiScreen]--;
	}
}

const zPosIVector& CSceneScreen::__GetNineScreen(const zPosI& uiPosI)
{
	const auto* pVecScreen = GetNineScreen(uiPosI);
	if (!pVecScreen || m_pMap->IsFullScreen())
		return GetFullScreen();
	else
		return *pVecScreen;
}

void CSceneScreen::__GetScreenByRange(const zPos& oPos, const int iRange, zPosIVector& vecPosI)
{
	int ox = oPos.x % SCREEN_GRID_WIDTH;
	int oy = oPos.y % SCREEN_GRID_HEIGHT;
	int nox = SCREEN_GRID_WIDTH - ox;
	int noy = SCREEN_GRID_HEIGHT - oy;
	int x = oPos.x / SCREEN_GRID_WIDTH;
	int y = oPos.y / SCREEN_GRID_HEIGHT;

	vecPosI.push_back(y*(int)zMap::uiMaxScreenX + x);
	if (iRange > oy)
	{
		if (y > 0)
		{
			vecPosI.push_back((y-1)*(int)zMap::uiMaxScreenX + x);
			if (x < (int)zMap::uiMaxScreenX && iRange > nox)
			{
				int dis = (int)sqrtf((float)(oy * oy) + float(nox * nox));
				if (iRange > dis)
				{
					vecPosI.push_back((y-1)*(int)zMap::uiMaxScreenX + x + 1);
				}
			}
		}
	}
	if (iRange > nox)
	{
		if (x < (int)zMap::uiMaxScreenX)
		{
			vecPosI.push_back(y*(int)zMap::uiMaxScreenX + x + 1);
			if (y < (int)zMap::uiMaxScreenY && iRange > noy)
			{
				int dis = (int)sqrtf((float)(nox * nox) + (float)(noy * noy));
				if (iRange > dis)
				{
					vecPosI.push_back((y + 1)*(int)zMap::uiMaxScreenX + x + 1);
				}
			}
		}
	}
	if (iRange > noy)
	{
		if (y < (int)zMap::uiMaxScreenX)
		{
			vecPosI.push_back((y + 1) * (int)zMap::uiMaxScreenX + x);
			if (x > 0 && iRange > ox)
			{
				int dis = (int)sqrtf((float)(noy * noy) + (float)(ox * ox));
				if (iRange < dis)
				{
					vecPosI.push_back((y + 1) * (int)zMap::uiMaxScreenX + x - 1);
				}
			}
		}
	}
	if (iRange > ox)
	{
		if (x > 0)
		{
			vecPosI.push_back(y * (int)zMap::uiMaxScreenX + x - 1);
			if (y > 0 && iRange > oy)
			{
				int dis = (int)sqrtf((float)(ox * ox) + (float)(oy * oy));
				if (iRange > dis)
				{
					vecPosI.push_back((y - 1) * (int)zMap::uiMaxScreenX + x - 1);
				}
			}
		}
	}
}

void CSceneScreen::__GetScreenByArea(const zPos& oLeftdown, const zPos& oRightup, zPosIVector& vecPosI)
{
	int leftx = oLeftdown.x / SCREEN_GRID_WIDTH;
	int lefty = oLeftdown.y / SCREEN_GRID_HEIGHT;
	int rightx = oRightup.x / SCREEN_GRID_WIDTH;
	int righty = oRightup.y / SCREEN_GRID_HEIGHT;

	for (auto x = leftx; x <= rightx; ++x)
	{
		for (auto y = lefty; y <= righty; ++y)
		{
			vecPosI.push_back(y * (int)zMap::uiMaxScreenX + x);
		}
	}
}

uint32 CSceneScreen::SizeNineRole(const zPosI uiScreen)
{
	uint32 uiCount = 0;
	const auto& vecScreen = __GetNineScreen(uiScreen);
	for (auto it = vecScreen.begin(); it != vecScreen.end(); ++it)
		uiCount += m_mapRoleCount[*it];
	return uiCount;
}

uint32 CSceneScreen::SizeNineMonster(const zPosI uiScreen)
{
	uint32 uiCount = 0;
	const auto& vecScreen = __GetNineScreen(uiScreen);
	for (auto it = vecScreen.begin(); it != vecScreen.end(); ++it)
		uiCount += m_mapMonsterCount[*it];
	return uiCount;
}

bool CSceneScreen::AddScreen(CCmptView* pView, const zPos& oNewPos)
{
	if (pView->HasInScene()) return false;

	auto type = pView->GetViewType();
	if (pView->SetPos(oNewPos))
	{
		zPosI uiScreen = pView->GetPosI();
		if (m_mapSetIndex[type][uiScreen].insert(pView).second)
		{
			m_mapVecIndex[type][uiScreen].push_back(pView);
		}
		++m_arrViewCount[type];
		m_setAll[type].insert(pView);
		pView->SetInScene();
		__AddScreenRole(uiScreen, pView);
		__AddScreenMonster(uiScreen, pView);
	}
	return pView->HasInScene();
}

bool CSceneScreen::FreshScreen(CCmptView* pView, const zPos& oNewPos)
{
	if (pView->HasInScene()) return false;
	
	bool bRet = false;
	auto type = pView->GetViewType();
	zPosI uiOld = pView->GetPosI();
	CmptViewSet& setViews = m_mapSetIndex[type][uiOld];
	CmptViewVec& vecViews = m_mapVecIndex[type][uiOld];
	CmptViewSet::const_iterator iter = setViews.find(pView);
	if (iter != setViews.end() && pView->SetPos(oNewPos))
	{
		bRet = true;
		zPosI uiNew = pView->GetPosI();
		if (uiOld != uiNew)
		{
			setViews.erase(iter);
			SimpleRemoveFromVec(vecViews, pView);
			if (m_mapSetIndex[type][uiNew].insert(pView).second)
				m_mapVecIndex[type][uiNew].push_back(pView);
			__SubScreenRole(uiOld, pView);
			__SubScreenMosnter(uiOld, pView);
			__AddScreenRole(uiNew, pView);
			__AddScreenMonster(uiNew, pView);
			pView->NotifyGatewayFreshScreen();
		}
	}
	return bRet;
}

bool CSceneScreen::RemoveScreen(CCmptView* pView)
{
	if (pView->HasInScene()) return false;

	auto type = pView->GetViewType();
	zPosI uiScreen = pView->GetPosI();
	CmptViewSet& setViews = m_mapSetIndex[type][uiScreen];
	CmptViewVec& vecViews = m_mapVecIndex[type][uiScreen];
	CmptViewSet::const_iterator iter = setViews.find(pView);
	if (iter != setViews.end())
	{
		setViews.erase(iter);
		SimpleRemoveFromVec(vecViews, pView);
		m_setAll[type].erase(pView);
		m_arrViewCount[type]--;
		__SubScreenRole(uiScreen, pView);
		pView->SetOutScene();
		return true;
	}
	return false;
}

void CSceneScreen::ExecAllOfScene(const uint32 uiType, CmptViewCallback& callback)
{
	for (auto it = m_setAll[uiType].begin(); it != m_setAll[uiType].end();)
	{
		auto tmp = it++;
		auto* pView = *tmp;
		if (!callback.exec(pView))
			return;
	}
}

bool CSceneScreen::ExecAllOfScene(const uint32 uiType, CmptViewFunc&& func)
{
	return std::all_of(m_setAll[uiType].begin(), m_setAll[uiType].end(), [&](auto* view) { return func(*view); });
}

void CSceneScreen::ExecAllOfScene(CmptViewCallback& callback)
{
	for (uint32 type = eViewType_Role; type < eViewType_Max; ++type)
		ExecAllOfScene(type, callback);
}

void CSceneScreen::ExecAllOfScene(CmptViewFunc&& func)
{
	for (uint32 type = eViewType_Role; type < eViewType_Max; ++type)
		ExecAllOfScene(type, CmptViewFunc(func));
}

void CSceneScreen::ExecAllOfScreen(const uint32 uiType, const zPosI uiScreen, CmptViewCallback& callback)
{
	CmptViewVec& vecViews = m_mapVecIndex[uiType][uiScreen];
	for (size_t i = 0; i < vecViews.size(); i++)
	{
		if (!callback.exec(vecViews[i]))
			return;
	}
}

void CSceneScreen::ExecAllOfScreen(const uint32 uiType, const zPosI uiScreen, CmptViewFunc&& func)
{
	CmptViewVec& vecViews = m_mapVecIndex[uiType][uiScreen];
	for (size_t i = 0; i < vecViews.size(); i++)
	{
		if (!func(*vecViews[i]))
			return;
	}
}

void CSceneScreen::ExecAllOfScreen(const zPosI uiScreen, CmptViewCallback& callback)
{
	for (uint32 type = eViewType_Role; type < eViewType_Max; ++type)
	{
		CmptViewVec& vecViews = m_mapVecIndex[type][uiScreen];
		for (size_t i = 0; i < vecViews.size(); i++)
		{
			if (!callback.exec(vecViews[i]))
				return;
		}
	}
}

void CSceneScreen::ExecAllOfScreen(const zPosI uiScreen, CmptViewFunc&& func)
{
	for (uint32 type = eViewType_Role; type < eViewType_Max; ++type)
	{
		CmptViewVec& vecViews = m_mapVecIndex[type][uiScreen];
		for (size_t i = 0; i < vecViews.size(); i++)
		{
			if (!func(*vecViews[i]))
				return;
		}
	}
}

void CSceneScreen::ExecAllOfNine(const uint32 uiType, const zPosI uiScreen, CmptViewCallback& callback)
{
	const auto& vecScreens = __GetNineScreen(uiScreen);
	for (auto it = vecScreens.begin(); it != vecScreens.end(); ++it)
	{
		ExecAllOfScreen(uiType, *it, callback);
	}
}

void CSceneScreen::ExecAllOfNine(const uint32 uiType, const zPosI uiScreen, CmptViewFunc&& func)
{
	const auto& vecScreens = __GetNineScreen(uiScreen);
	std::for_each(vecScreens.begin(), vecScreens.end(), [&](auto posI) { ExecAllOfScreen(uiType, posI, std::move(func)); });
}

void CSceneScreen::ExecAllOfNine(const zPosI uiScreen, CmptViewCallback& callback)
{
	const auto& vecScreens = __GetNineScreen(uiScreen);
	for (auto it = vecScreens.begin(); it != vecScreens.end(); ++it)
	{
		ExecAllOfScreen(*it, callback);
	}
}

void CSceneScreen::ExecAllOfNine(const zPosI uiScreen, CmptViewFunc&& func)
{
	const auto& vecScreens = __GetNineScreen(uiScreen);
	std::for_each(vecScreens.begin(), vecScreens.end(), [&](auto posI) { ExecAllOfScreen(posI, std::move(func)); });
}

void CSceneScreen::ExecAllOfDirect(const uint32 uiType, const zPosI uiScreen, const int direct, CmptViewCallback& callback)
{
	const auto* pVecScreens = GetDirectScreen(uiScreen, direct);
	if (m_pMap->IsFullScreen() || !pVecScreens)
		return;
	for (auto it = pVecScreens->begin(); it != pVecScreens->end();++it)
	{
		ExecAllOfScreen(uiType, *it, callback);
	}
}

void CSceneScreen::ExecAllOfDirect(const zPosI uiScreen, const int direct, CmptViewCallback& callback)
{
	const auto* pVecScreens = GetDirectScreen(uiScreen, direct);
	if (m_pMap->IsFullScreen() || !pVecScreens)
		return;
	for (auto it = pVecScreens->begin(); it != pVecScreens->end(); ++it)
	{
		ExecAllOfScreen(*it, callback);
	}
}

void CSceneScreen::ExecAllOfRDirect(const uint32 uiType, const zPosI uiScreen, const int direct, CmptViewCallback& callback)
{
	const auto* pVecScreens = GetReverseDirectScreen(uiScreen, direct);
	if (m_pMap->IsFullScreen() || !pVecScreens)
		return;
	for (auto it = pVecScreens->begin(); it != pVecScreens->end(); ++it)
	{
		ExecAllOfScreen(uiType, *it, callback);
	}
}

void CSceneScreen::ExecAllOfRDirect(const zPosI uiScreen, const int direct, CmptViewCallback& callback)
{
	const auto* pVecScreens = GetReverseDirectScreen(uiScreen, direct);
	if (m_pMap->IsFullScreen() || !pVecScreens)
		return;
	for (auto it = pVecScreens->begin(); it != pVecScreens->end(); ++it)
	{
		ExecAllOfScreen(*it, callback);
	}
}

void CSceneScreen::ExecAllOfPosRange(const zPos& oPos, const int iRange, CmptViewCallback& callback)
{
	zPosIVector vecScreen;
	if (m_pMap->IsFullScreen())
		vecScreen = GetFullScreen();
	else
		__GetScreenByRange(oPos, iRange, vecScreen);
	for (zPosIVector::const_iterator it = vecScreen.begin(); it != vecScreen.end(); ++it)
	{
		ExecAllOfScreen(*it, callback);
	}
}

void CSceneScreen::ExecAllOfPosRange(const uint32 uiType, const zPos& oPos, const int iRange, CmptViewCallback& callback)
{
	zPosIVector vecScreen;
	if (m_pMap->IsFullScreen())
		vecScreen = GetFullScreen();
	else
		__GetScreenByRange(oPos, iRange, vecScreen);
	for (zPosIVector::const_iterator it = vecScreen.begin(); it != vecScreen.end(); ++it)
	{
		ExecAllOfScreen(uiType, *it, callback);
	}
}

void CSceneScreen::ExecAllOfPosArea(const zPos& oLeftdown, const zPos& oRightup, CmptViewCallback& callback)
{
	zPosIVector vecScreen;
	if (m_pMap->IsFullScreen())
		vecScreen = GetFullScreen();
	else
		__GetScreenByArea(oLeftdown, oRightup, vecScreen);
	for (zPosIVector::const_iterator it = vecScreen.begin(); it != vecScreen.end(); ++it)
	{
		ExecAllOfScreen(*it, callback);
	}
}

void CSceneScreen::ExecAllOfPosArea(const uint32 uiType, const zPos& oLeftdown, const zPos& oRightup, CmptViewCallback& callback)
{
	zPosIVector vecScreen;
	if (m_pMap->IsFullScreen())
		vecScreen = GetFullScreen();
	else
		__GetScreenByArea(oLeftdown, oRightup, vecScreen);
	for (zPosIVector::const_iterator it = vecScreen.begin(); it != vecScreen.end(); ++it)
	{
		ExecAllOfScreen(uiType, *it, callback);
	}
}