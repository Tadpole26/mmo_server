#pragma once

#include "sdk.h"

enum eViewType
{
	eViewType_Role,						//��ͼ����
	eViewType_Npc,						//��ͼ����
	eViewType_Max		
};

class CCmptView;
class CSceneMap;
using CmptViewCallback = zCallback<CCmptView, bool>;
class CSceneScreen : public zScreenIndex
{
public:
	using CmptViewSet = std::set<CCmptView*, std::less<CCmptView*>>;
	using CmptViewVec = std::vector<CCmptView*>;
	using CmptViewFunc = std::function<bool(CCmptView&)>;
	using PosISetMap = std::map<zPosI, CmptViewSet, std::less<zPosI>>;
	using PosIVecMap = std::map<zPosI, CmptViewVec, std::less<zPosI>>;
	using PosIRoleMap = std::map<zPosI, uint32, std::less<zPosI>>;
	using PosIMonsterMap = std::map<zPosI, uint32, std::less<zPosI>>;
public:
	static const zPosIVector& GetFullScreen();
	static const zPosIVector& GetEmptyScreen();
public:
	CSceneScreen();
	~CSceneScreen();
public:
	inline size_t SizeView(eViewType eType) const { return m_arrViewCount[eType]; }
	inline uint32 SizeScreenRole(const zPosI uiScreen) { return m_mapRoleCount[uiScreen]; }
	inline uint32 SizeScreenMonster(const zPosI uiScreen) { return m_mapMonsterCount[uiScreen]; }
	uint32 SizeNineRole(const zPosI uiScreen);
	uint32 SizeNineMonster(const zPosI uiScreen);
	bool AddScreen(CCmptView* pView, const zPos& oNewPos);
	//ˢ����(�������л�)
	bool FreshScreen(CCmptView* pView, const zPos& oNewPos);
	bool RemoveScreen(CCmptView* pView);
public:
	void ExecAllOfScene(const uint32 uiType, CmptViewCallback& callback);
	bool ExecAllOfScene(const uint32 uiType, CmptViewFunc&& func);
	void ExecAllOfScene(CmptViewCallback& callback);
	void ExecAllOfScene(CmptViewFunc&& func);
	void ExecAllOfScreen(const uint32 uiType, const zPosI uiScreen, CmptViewCallback& callback);
	void ExecAllOfScreen(const uint32 uiType, const zPosI uiScreen, CmptViewFunc&& func);
	void ExecAllOfScreen(const zPosI uiScreen, CmptViewCallback& callback);
	void ExecAllOfScreen(const zPosI uiScreen, CmptViewFunc&& func);
	void ExecAllOfNine(const uint32 uiType, const zPosI uiScreen, CmptViewCallback& callback);
	void ExecAllOfNine(const uint32 uiType, const zPosI uiScreen, CmptViewFunc&& func);
	void ExecAllOfNine(const zPosI uiScreen, CmptViewCallback& callback);
	void ExecAllOfNine(const zPosI uiScreen, CmptViewFunc&& func);
	void ExecAllOfDirect(const uint32 uiType, const zPosI uiScreen, const int direct, CmptViewCallback& callback);
	void ExecAllOfDirect(const zPosI uiScreen, const int direct, CmptViewCallback& callback);
	void ExecAllOfRDirect(const uint32 uiType, const zPosI uiScreen, const int direct, CmptViewCallback& callback);
	void ExecAllOfRDirect(const zPosI uiScreen, const int direct, CmptViewCallback& callback);
	void ExecAllOfPosRange(const zPos& oPos, const int iRange, CmptViewCallback& callback);
	void ExecAllOfPosRange(const uint32 uiType, const zPos& oPos, const int iRange, CmptViewCallback& callback);
	void ExecAllOfPosArea(const zPos& oLeftdown, const zPos& oRightup, CmptViewCallback& callback);
	void ExecAllOfPosArea(const uint32 uiType, const zPos& oLeftdown, const zPos& oRightup, CmptViewCallback& callback);
private:
	void __AddScreenRole(const zPosI uiScreen, CCmptView* pView);
	void __SubScreenRole(const zPosI uiScreen, CCmptView* pView);
	void __AddScreenMonster(const zPosI uiScreen, CCmptView* pView);
	void __SubScreenMosnter(const zPosI uiScreen, CCmptView* pView);
	const zPosIVector& __GetNineScreen(const zPosI& uiPosI);
	void __GetScreenByRange(const zPos& oPos, const int iRange, zPosIVector& vecPosI);
	void __GetScreenByArea(const zPos& oLeftdown, const zPos& oRightup, zPosIVector& vecPosI);
public:
	CSceneMap* m_pMap = nullptr;
private:
	size_t m_arrViewCount[eViewType_Max];								//��Ұ���ͼ���(������ͼ��Ұ��ɫ���͹�����)
	PosISetMap m_mapSetIndex[eViewType_Max];							//ÿ����ɫ�͹���set
	PosIVecMap m_mapVecIndex[eViewType_Max];							//ÿ����ɫ�͹���vec
	CmptViewSet m_setAll[eViewType_Max];								//������ͼ��Ұ��ɫ�͹���
	PosIRoleMap m_mapRoleCount;											//ÿ�����Ǽ���
	PosIMonsterMap m_mapMonsterCount;									//ÿ���������
};