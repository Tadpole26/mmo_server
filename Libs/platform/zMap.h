#pragma once
#include "sdk.h"

//��ͼ������Ϣ
class zMap
{

public:
	//�������(��������е�ͼ�������)
	static zPos2 bigWH;
	//���������(��������е�ͼ�������)
	static uint32 screenX;
	//���������(��������е�ͼ�������)
	static uint32 screenY;
public:
	//��������������Ƿ�����Ұ��ϵ
	static bool checkScreenIdInView(const zPosI uiOld, const zPosI uiNew);
	//���������������õ�������[0(��)|1(����)|2(��)|3(����)|4(��)|5(����)|6(��)|7(����)]
	static uint16 getScreenDirect(const zPosI uiSrc, const zPosI uiDst);
public:
	zMap(uint32 uiMapId);
	virtual ~zMap();
	virtual void final();

	inline uint32 getMapId() const { return _mapId; }
	inline const zPos2 getPosWH() { return _posWH; }
	//��ȡ��ͼ��
	inline const uint32 getWidth() { return _posWH.x; }
	//��ȡ��ͼ��
	inline const uint32 getHeight() { return _posWH.y; }
	//�Ƿ���Ч��x
	bool validX(const uint32 uiX) { return uiX <= _posWH.x; }
	//�Ƿ���Ч��y
	bool validY(const uint32 uiY) { return uiY <= _posWH.y; }
	//�Ƿ���Ч��zPos
	bool validPos(const zPos& oPos) { return (oPos.x <= _posWH.x && oPos.y <= _posWH.y); }
	//�Ƿ���Ч��grideId
	bool validGridId(const uint32 uiGrideId);
	//x,yתgridId
	const uint32 xy2GridId(const uint32 uiX, const uint32 uiY);
	//zPosתgridId
	const uint32 pos2GridId(const zPos& oPos);
	//gridIdתx
	const uint32 gridId2X(const uint32 uiGridId);
	//grideIdתy
	const uint32 gridId2Y(const uint32 uiGridId);
protected:
	//���ÿ�
	void setWidth(const uint32 uiWidth) { _posWH.x = uiWidth; }
	//���ø�
	void setHeight(const uint32 uiHeight) { _posWH.y = uiHeight; }
private:
	uint32 _mapId = 0;
	//��ͼʵ��������� 
	zPos2 _posWH;
};
