#include "zMap.h"

zMap::zMap(uint32 uiMapId) : m_uiMapId(uiMapId)
{
}

zMap::~zMap()
{
}

void zMap::Final()
{
}

bool zMap::CheckScreenIdInView(const zPosI uiOld, const zPosI uiNew)
{
	uint32 uiOldX = uiOld % zMap::uiMaxScreenX;
	uint32 uiOldY = uiOld / zMap::uiMaxScreenX;
	uint32 uiMaxOldX = uiOldX + 1;
	uint32 uiMinOldX = uiOldX > 1 ? uiOldX - 1 : 0;
	uint32 uiMaxOldY = uiOldY + 1;
	uint32 uiMinOldY = uiOldY > 1 ? uiOldY - 1 : 0;
	uint32 uiNewX = uiNew % zMap::uiMaxScreenX;
	uint32 uiNewY = uiNew / zMap::uiMaxScreenX;
	return (uiNewX >= uiMinOldX && uiNewX <= uiMaxOldX && uiNewY >= uiMinOldY && uiNewY <= uiMaxOldX);
}

uint16 zMap::GetScreenDirect(const zPosI uiSrc, const zPosI uiDst)
{
	uint32 uiSrcX = uiSrc % zMap::uiMaxScreenX;
	uint32 uiSrcY = uiSrc / zMap::uiMaxScreenX;
	uint32 uiDstX = uiDst % zMap::uiMaxScreenX;
	uint32 uiDstY = uiDst / zMap::uiMaxScreenX;

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

bool zMap::ValidGridId(const uint32 uiGrideId)
{
	uint32 uiSubY = m_oPosWH.y > 1 ? m_oPosWH.y - 1 : 0;
	return uiGrideId < (uiSubY * m_oPosWH.x + m_oPosWH.x);
}

const uint32 zMap::Xy2GridId(const uint32 uiX, const uint32 uiY)
{
	uint32 uiSubY = uiY > 1 ? uiY - 1 : 0;
	return (uiSubY * m_oPosWH.x + uiX);
}

const uint32 zMap::Pos2GridId(const zPos& oPos)
{
	uint32 uiSubY = oPos.y > 1 ? oPos.y - 1 : 0;
	return (uiSubY * m_oPosWH.x + oPos.x);
}

const uint32 zMap::GridId2X(const uint32 uiGridId)
{
	return uiGridId % m_oPosWH.x;
}

const uint32 zMap::GridId2Y(const uint32 uiGridId)
{
	return (uiGridId / m_oPosWH.x + 1);
}