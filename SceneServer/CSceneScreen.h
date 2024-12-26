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
class CSceneScreen : public zScreenIndex
{
public:
	using CmptViewSet = std::set<CCmptView*, std::less<CCmptView*>>;
	using CmptViewVec = std::vector<CCmptView*>;
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
	inline uint32 SizeView(eViewType eType) const { return m_arrViewCount[eType]; }
	inline uint32 
private:
	size_t m_arrViewCount[eViewType_Max];								//��Ұ���ͼ���(������ͼ��Ұ��ɫ���͹�����)
	PosISetMap m_mapSetIndex[eViewType_Max];							//ÿ����ɫ�͹���set
	PosIVecMap m_mapVecIndex[eViewType_Max];							//ÿ����ɫ�͹���vec
	CmptViewSet m_setAll[eViewType_Max];								//������ͼ��Ұ��ɫ�͹���
	PosIRoleMap m_mapRoleCount;											//ÿ�����Ǽ���
	PosIMonsterMap m_mapMonsterCount;									//ÿ���������
};