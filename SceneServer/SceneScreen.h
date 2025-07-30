#pragma once

#include "sdk.h"

enum eViewType
{
	eViewType_Role,						//地图主角
	eViewType_Npc,						//地图怪物
	eViewType_Max		
};

class CmptView;
class SceneMap;
using CmptViewCallback = zCallback<CmptView, bool>;
class SceneScreen : public zScreenIndex
{
public:
	using CmptViewSetT = std::set<CmptView*, std::less<CmptView*>>;
	using CmptViewVecT = std::vector<CmptView*>;
	using CmptViewFuncT = std::function<bool(CmptView&)>;
	using PosIMapSetT = std::map<zPosI, CmptViewSetT, std::less<zPosI>>;
	using PosIMapVecT = std::map<zPosI, CmptViewVecT, std::less<zPosI>>;
	using PosIMapRoleT = std::map<zPosI, uint32, std::less<zPosI>>;
	using PosIMapMonsterT = std::map<zPosI, uint32, std::less<zPosI>>;
public:
	static const zPosIVector& getFullScreen();
	static const zPosIVector& getEmptyScreen();
public:
	SceneScreen();
	~SceneScreen();
public:
	inline size_t sizeView(eViewType eType) const { return _viewcount[eType]; }
	inline uint32 sizeScreenRole(const zPosI uiScreen) { return _rolecount[uiScreen]; }
	inline uint32 sizeScreenMonster(const zPosI uiScreen) { return _monstercount[uiScreen]; }
	uint32 sizeNineRole(const zPosI uiScreen);
	uint32 sizeNineMonster(const zPosI uiScreen);
	bool addScreen(CmptView* pView, const zPos& oNewPos);
	//刷新屏(可能屏切换)
	bool freshScreen(CmptView* pView, const zPos& oNewPos);
	bool removeScreen(CmptView* pView);
public:
	void execAllOfScene(const uint32 uiType, CmptViewCallback& callback);
	bool execAllOfScene(const uint32 uiType, CmptViewFuncT&& func);
	void execAllOfScene(CmptViewCallback& callback);
	void execAllOfScene(CmptViewFuncT&& func);
	void execAllOfScreen(const uint32 uiType, const zPosI uiScreen, CmptViewCallback& callback);
	void execAllOfScreen(const uint32 uiType, const zPosI uiScreen, CmptViewFuncT&& func);
	void execAllOfScreen(const zPosI uiScreen, CmptViewCallback& callback);
	void execAllOfScreen(const zPosI uiScreen, CmptViewFuncT&& func);
	void execAllOfNine(const uint32 uiType, const zPosI uiScreen, CmptViewCallback& callback);
	void execAllOfNine(const uint32 uiType, const zPosI uiScreen, CmptViewFuncT&& func);
	void execAllOfNine(const zPosI uiScreen, CmptViewCallback& callback);
	void execAllOfNine(const zPosI uiScreen, CmptViewFuncT&& func);
	void execAllOfDirect(const uint32 uiType, const zPosI uiScreen, const int direct, CmptViewCallback& callback);
	void execAllOfDirect(const zPosI uiScreen, const int direct, CmptViewCallback& callback);
	void execAllOfRDirect(const uint32 uiType, const zPosI uiScreen, const int direct, CmptViewCallback& callback);
	void execAllOfRDirect(const zPosI uiScreen, const int direct, CmptViewCallback& callback);
	void execAllOfPosRange(const zPos& oPos, const int iRange, CmptViewCallback& callback);
	void execAllOfPosRange(const uint32 uiType, const zPos& oPos, const int iRange, CmptViewCallback& callback);
	void execAllOfPosArea(const zPos& oLeftdown, const zPos& oRightup, CmptViewCallback& callback);
	void execAllOfPosArea(const uint32 uiType, const zPos& oLeftdown, const zPos& oRightup, CmptViewCallback& callback);
private:
	void __addScreenRole(const zPosI uiScreen, CmptView* pView);
	void __subScreenRole(const zPosI uiScreen, CmptView* pView);
	void __addScreenMonster(const zPosI uiScreen, CmptView* pView);
	void __subScreenMosnter(const zPosI uiScreen, CmptView* pView);
	const zPosIVector& __getNineScreen(const zPosI& uiPosI);
	void __getScreenByRange(const zPos& oPos, const int iRange, zPosIVector& vecPosI);
	void __getScreenByArea(const zPos& oLeftdown, const zPos& oRightup, zPosIVector& vecPosI);
public:
	SceneMap* map = nullptr;
private:
	size_t _viewcount[eViewType_Max];								//视野类型计数(整个地图视野角色数和怪物数)
	PosIMapVecT _indexvec[eViewType_Max];							//每屏角色和怪物set
	PosIMapSetT _indexset[eViewType_Max];							//每屏角色和怪物vec
	CmptViewSetT _allset[eViewType_Max];								//整个地图视野角色和怪物
	PosIMapRoleT _rolecount;											//每屏主角计数
	PosIMapMonsterT _monstercount;									//每屏怪物计数
};