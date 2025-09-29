#include "zMap.h"

uint32 zMap::screenX = 69;
uint32 zMap::screenY = 69;
zPos2 zMap::bigWH(2048.500000, 2048.500000);
zMap::zMap(uint32 uiMapId) : _mapId(uiMapId)
{
}

zMap::~zMap()
{
}

void zMap::final()
{
}

bool zMap::checkScreenIdInView(const zPosI uiOld, const zPosI uiNew)
{
	uint32 uiOldX = uiOld % zMap::screenX;
	uint32 uiOldY = uiOld / zMap::screenX;
	uint32 uiMaxOldX = uiOldX + 1;
	uint32 uiMinOldX = uiOldX > 1 ? uiOldX - 1 : 0;
	uint32 uiMaxOldY = uiOldY + 1;
	uint32 uiMinOldY = uiOldY > 1 ? uiOldY - 1 : 0;
	uint32 uiNewX = uiNew % zMap::screenX;
	uint32 uiNewY = uiNew / zMap::screenX;
	return (uiNewX >= uiMinOldX && uiNewX <= uiMaxOldX && uiNewY >= uiMinOldY && uiNewY <= uiMaxOldX);
}

uint16 zMap::getScreenDirect(const zPosI uiSrc, const zPosI uiDst)
{
	uint32 uiSrcX = uiSrc % zMap::screenX;
	uint32 uiSrcY = uiSrc / zMap::screenX;
	uint32 uiDstX = uiDst % zMap::screenX;
	uint32 uiDstY = uiDst / zMap::screenX;

	uint32 uiSubY = uiSrcY > 1 ? uiSrcY - 1 : 0;
	uint32 uiAddY = uiSrcY + 1;
	uint32 uiSubX = uiSrcX > 1 ? uiSrcX - 1 : 0;
	uint32 uiAddX = uiSrcX + 1;
	//0(下) 
	if (uiSrcX == uiDstX && uiSubY == uiDstY)
		return 0;
	//1(右下)
	else if (uiAddX == uiDstX && uiSubY == uiDstY)
		return 1;
	//2(右)
	else if (uiAddX == uiDstX && uiSrcY == uiDstY)
		return 2;
	//3(右上)
	else if (uiAddX == uiDstX && uiAddY == uiDstY)
		return 3;
	//4(上)
	else if (uiSrcX == uiDstX && uiAddY == uiDstY)
		return 4;
	//5(左上)
	else if (uiSubX == uiDstX && uiAddY == uiDstY)
		return 5;
	//6(左)
	else if (uiSubX == uiDstX && uiSrcY == uiDstY)
		return 6;
	//7(左下)
	else /*if (uiSubX == uiDstX && uiSubY == uiDstY)*/
		return 7;
}

bool zMap::validGridId(const uint32 uiGrideId)
{
	uint32 uiSubY = _posWH.y > 1 ? _posWH.y - 1 : 0;
	return uiGrideId < (uiSubY * _posWH.x + _posWH.x);
}

const uint32 zMap::xy2GridId(const uint32 uiX, const uint32 uiY)
{
	uint32 uiSubY = uiY > 1 ? uiY - 1 : 0;
	return (uiSubY * _posWH.x + uiX);
}

const uint32 zMap::pos2GridId(const zPos& oPos)
{
	uint32 uiSubY = oPos.y > 1 ? oPos.y - 1 : 0;
	return (uiSubY * _posWH.x + oPos.x);
}

const uint32 zMap::gridId2X(const uint32 uiGridId)
{
	return uiGridId % _posWH.x;
}

const uint32 zMap::gridId2Y(const uint32 uiGridId)
{
	return (uiGridId / _posWH.x + 1);
}