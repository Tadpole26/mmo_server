#pragma once
#include "sdk.h"

//��ͼ������Ϣ
class zMap
{

public:
	//�������(��������е�ͼ�������)
	static zPos2 oBigWH;
	//���������(��������е�ͼ�������)
	static uint32 uiMaxScreenX;
	//���������(��������е�ͼ�������)
	static uint32 uiMaxScreenY;
public:
	//��������������Ƿ�����Ұ��ϵ
	static bool CheckScreenIdInView(const zPosI uiOld, const zPosI uiNew);
	//���������������õ�������[0(��)|1(����)|2(��)|3(����)|4(��)|5(����)|6(��)|7(����)]
	static uint16 GetScreenDirect(const zPosI uiSrc, const zPosI uiDst);
public:
	zMap(uint32 uiMapId);
	virtual ~zMap();
	virtual void Final();

	inline uint32 GetMapId() { return m_uiMapId; }
	inline const zPos2 GetPosWH() { return m_oPosWH; }
	//��ȡ��ͼ��
	inline const uint32 GetWidth() { return m_oPosWH.x; }
	//��ȡ��ͼ��
	inline const uint32 GetHeight() { return m_oPosWH.y; }
	//�Ƿ���Ч��x
	bool ValidX(const uint32 uiX) { return uiX <= m_oPosWH.x; }
	//�Ƿ���Ч��y
	bool ValidY(const uint32 uiY) { return uiY <= m_oPosWH.y; }
	//�Ƿ���Ч��zPos
	bool ValidPos(const zPos& oPos) { return (oPos.x <= m_oPosWH.x && oPos.y <= m_oPosWH.y); }
	//�Ƿ���Ч��grideId
	bool ValidGridId(const uint32 uiGrideId);
	//x,yתgridId
	const uint32 Xy2GridId(const uint32 uiX, const uint32 uiY);
	//zPosתgridId
	const uint32 Pos2GridId(const zPos& oPos);
	//gridIdתx
	const uint32 GridId2X(const uint32 uiGridId);
	//grideIdתy
	const uint32 GridId2Y(const uint32 uiGridId);
	//���ÿ�
	void SetWidth(const uint32 uiWidth) { m_oPosWH.x = uiWidth; }
	//���ø�
	void SetHeight(const uint32 uiHeight) { m_oPosWH.y = uiHeight; }
private:
	uint32 m_uiMapId = 0;
	//��ͼʵ��������� 
	zPos2 m_oPosWH;
};
