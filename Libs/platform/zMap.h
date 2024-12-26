#pragma once
#include "sdk.h"

//地图基本信息
class zMap
{

public:
	//最大坐标(估算的所有地图最大坐标)
	static zPos2 oBigWH;
	//横向多少屏(估算的所有地图最大屏数)
	static uint32 uiMaxScreenX;
	//纵向多少屏(估算的所有地图最大屏数)
	static uint32 uiMaxScreenY;
public:
	//检查两个屏索引是否有视野关系
	static bool CheckScreenIdInView(const zPosI uiOld, const zPosI uiNew);
	//根据两个屏索引得到屏方向[0(下)|1(右下)|2(右)|3(右上)|4(上)|5(左上)|6(左)|7(左下)]
	static uint16 GetScreenDirect(const zPosI uiSrc, const zPosI uiDst);
public:
	zMap(uint32 uiMapId);
	virtual ~zMap();
	virtual void Final();

	inline uint32 GetMapId() { return m_uiMapId; }
	inline const zPos2 GetPosWH() { return m_oPosWH; }
	//获取地图宽
	inline const uint32 GetWidth() { return m_oPosWH.x; }
	//获取地图高
	inline const uint32 GetHeight() { return m_oPosWH.y; }
	//是否有效的x
	bool ValidX(const uint32 uiX) { return uiX <= m_oPosWH.x; }
	//是否有效的y
	bool ValidY(const uint32 uiY) { return uiY <= m_oPosWH.y; }
	//是否有效的zPos
	bool ValidPos(const zPos& oPos) { return (oPos.x <= m_oPosWH.x && oPos.y <= m_oPosWH.y); }
	//是否有效的grideId
	bool ValidGridId(const uint32 uiGrideId);
	//x,y转gridId
	const uint32 Xy2GridId(const uint32 uiX, const uint32 uiY);
	//zPos转gridId
	const uint32 Pos2GridId(const zPos& oPos);
	//gridId转x
	const uint32 GridId2X(const uint32 uiGridId);
	//grideId转y
	const uint32 GridId2Y(const uint32 uiGridId);
	//设置宽
	void SetWidth(const uint32 uiWidth) { m_oPosWH.x = uiWidth; }
	//设置高
	void SetHeight(const uint32 uiHeight) { m_oPosWH.y = uiHeight; }
private:
	uint32 m_uiMapId = 0;
	//地图实际最大坐标 
	zPos2 m_oPosWH;
};
