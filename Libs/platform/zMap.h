#pragma once
#include "sdk.h"

//地图基本信息
class zMap
{

public:
	//最大坐标(估算的所有地图最大坐标)
	static zPos2 bigWH;
	//横向多少屏(估算的所有地图最大屏数)
	static uint32 screenX;
	//纵向多少屏(估算的所有地图最大屏数)
	static uint32 screenY;
public:
	//检查两个屏索引是否有视野关系
	static bool checkScreenIdInView(const zPosI uiOld, const zPosI uiNew);
	//根据两个屏索引得到屏方向[0(下)|1(右下)|2(右)|3(右上)|4(上)|5(左上)|6(左)|7(左下)]
	static uint16 getScreenDirect(const zPosI uiSrc, const zPosI uiDst);
public:
	zMap(uint32 uiMapId);
	virtual ~zMap();
	virtual void final();

	inline uint32 getMapId() const { return _mapId; }
	inline const zPos2 getPosWH() { return _posWH; }
	//获取地图宽
	inline const uint32 getWidth() { return _posWH.x; }
	//获取地图高
	inline const uint32 getHeight() { return _posWH.y; }
	//是否有效的x
	bool validX(const uint32 uiX) { return uiX <= _posWH.x; }
	//是否有效的y
	bool validY(const uint32 uiY) { return uiY <= _posWH.y; }
	//是否有效的zPos
	bool validPos(const zPos& oPos) { return (oPos.x <= _posWH.x && oPos.y <= _posWH.y); }
	//是否有效的grideId
	bool validGridId(const uint32 uiGrideId);
	//x,y转gridId
	const uint32 xy2GridId(const uint32 uiX, const uint32 uiY);
	//zPos转gridId
	const uint32 pos2GridId(const zPos& oPos);
	//gridId转x
	const uint32 gridId2X(const uint32 uiGridId);
	//grideId转y
	const uint32 gridId2Y(const uint32 uiGridId);
protected:
	//设置宽
	void setWidth(const uint32 uiWidth) { _posWH.x = uiWidth; }
	//设置高
	void setHeight(const uint32 uiHeight) { _posWH.y = uiHeight; }
private:
	uint32 _mapId = 0;
	//地图实际最大坐标 
	zPos2 _posWH;
};
