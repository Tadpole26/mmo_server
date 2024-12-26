#include "zScreenIndex.h"


//构造
zScreenIndex::zScreenIndex()
{
}

//析构
zScreenIndex::~zScreenIndex()
{
}

static bool GetDirectIndex(uint32_t x, uint32_t y, uint32_t direct, uint32_t& index)
{
	if (direct == 0)						//下屏
	{
		if (y <= 0) return false;
		index = (y - 1) * zMap::uiMaxScreenX + x;
	}
	else if (direct == 1)					//右下屏
	{
		if (x >= (zMap::uiMaxScreenX - 1) || y <= 0) return false;
		index = (y - 1) * zMap::uiMaxScreenX + (x + 1);
	}
	else if (direct == 2)					//右
	{
		if (x >= (zMap::uiMaxScreenX - 1)) return false;
		index = y * zMap::uiMaxScreenX + (x + 1);
	}
	else if (direct == 3)					//右上
	{
		if (x >= (zMap::uiMaxScreenX - 1) || y >= (zMap::uiMaxScreenX - 1)) return false;
		index = (y + 1) * zMap::uiMaxScreenX + (x + 1);
	}
	else if (direct == 4)					//上
	{
		if (y >= (zMap::uiMaxScreenX - 1)) return false;
		index = (y + 1) * zMap::uiMaxScreenX + x;
	}
	else if (direct == 5)					//左上
	{
		if (x <= 0 || y >= (zMap::uiMaxScreenX - 1)) return false;
		index = (y + 1) * zMap::uiMaxScreenX + (x - 1);
	}
	else if (direct == 6)					//左
	{
		if (x <= 0) return false;
		index = y * zMap::uiMaxScreenX + (x - 1);
	}
	else if (direct == 7)					//左下
	{
		if (x <= 0 || y <= 0) return false;
		index = (y - 1) * zMap::uiMaxScreenX + (x - 1);
	}
	else
		return false;
	return true;
}

bool zScreenIndex::Init()
{
	//初始化地图最大坐标,和地图最大数
	zMap::oBigWH.x = 2048;
	zMap::oBigWH.y = 2048;
	zMap::uiMaxScreenX = 69;
	zMap::uiMaxScreenY = 69;

	for (uint32 y = 0; y < zMap::uiMaxScreenY; ++y)
	{
		for (uint32 x = 0; x < zMap::uiMaxScreenX; ++x)
		{
			uint32 uiCurIndex = y * zMap::uiMaxScreenX + x;
			//向下(前向屏索引)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[0][uiCurIndex];
				uint32 uiDirIndex = 0;
				//左下屏
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//下屏
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右下屏
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//右下(前向屏索引)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[1][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左下屏
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//下屏
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右下屏
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右屏
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右上屏
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//右(前向屏索引)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[2][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//右上屏
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右屏
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右下屏
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//右上(前向屏索引)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[3][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左上屏
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//上屏
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右上屏
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右屏
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右下屏
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//上(前向屏索引)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[4][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左上屏
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//上屏
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右上屏
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//左上(前向屏索引)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[5][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左下屏
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左屏
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左上屏
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//上屏
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右上屏
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//左(前向屏索引)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[6][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左下屏
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左屏
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左上屏
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//左下(前向屏索引)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[7][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左上屏
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左屏
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左下屏
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//下屏
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右下屏
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}

			//向下(后向屏索引)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[0][uiCurIndex];
				uint32 uiDirIndex = 0;
				//左上屏
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//上屏
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右上屏
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//右下(后向屏索引)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[1][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左下屏
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左屏
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左上屏
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//上屏
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右上屏
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//右(后向屏索引)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[2][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左下屏
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左屏
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左上屏
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//右上(后向屏索引)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[3][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左上屏
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左屏
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左下屏
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//下屏
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右下屏
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//上(后向屏索引)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[4][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左下屏
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//下屏
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右下屏
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//左上(后向屏索引)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[5][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//右上屏
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右屏
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右下屏
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//下屏
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//左下屏
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//左(后向屏索引)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[6][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//右上屏
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右屏
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右下屏
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//左下(后向屏索引)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[7][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//左上屏
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//上屏
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右上屏
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右屏
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//右下屏
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}

			//九屏
			{
				std::vector<uint32_t>& vecIndex = nine_screen[uiCurIndex];
				uint32_t uiDirIndex = 0;
				for (uint32 i = 0; i < 8; ++i)
				{
					if (GetDirectIndex(x, y, i, uiDirIndex))
						vecIndex.push_back(uiDirIndex);
				}
			}
		}
	}
	return true;
}

//根据屏索引得到九屏索引
const zPosIVector* zScreenIndex::GetNineScreen(const zPosI uiPos)
{
	auto iterFind = nine_screen.find(uiPos);
	if (iterFind == nine_screen.end())
		return nullptr;
	return &(iterFind->second);
}

//根据屏索引和方向得到前向屏索引
const zPosIVector* zScreenIndex::GetDirectScreen(const zPosI uiPos, const int direct)
{
	if (direct > 7)
		return nullptr;
	nineScreen_map& mapScreen = direct_screen[direct];
	auto iterFind = mapScreen.find(uiPos);
	if (iterFind == mapScreen.end())
		return nullptr;
	return &(iterFind->second);
}

//根据屏索引和方向得到后向屏索引
const zPosIVector* zScreenIndex::GetReverseDirectScreen(const zPosI uiPos, const int direct)
{
	if (direct > 7)
		return nullptr;
	nineScreen_map& mapScreen = reversedirect_screen[direct];
	auto iterFind = mapScreen.find(uiPos);
	if (iterFind == mapScreen.end())
		return nullptr;
	return &(iterFind->second);
}
