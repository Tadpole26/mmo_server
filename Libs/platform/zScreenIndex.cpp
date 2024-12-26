#include "zScreenIndex.h"


//����
zScreenIndex::zScreenIndex()
{
}

//����
zScreenIndex::~zScreenIndex()
{
}

static bool GetDirectIndex(uint32_t x, uint32_t y, uint32_t direct, uint32_t& index)
{
	if (direct == 0)						//����
	{
		if (y <= 0) return false;
		index = (y - 1) * zMap::uiMaxScreenX + x;
	}
	else if (direct == 1)					//������
	{
		if (x >= (zMap::uiMaxScreenX - 1) || y <= 0) return false;
		index = (y - 1) * zMap::uiMaxScreenX + (x + 1);
	}
	else if (direct == 2)					//��
	{
		if (x >= (zMap::uiMaxScreenX - 1)) return false;
		index = y * zMap::uiMaxScreenX + (x + 1);
	}
	else if (direct == 3)					//����
	{
		if (x >= (zMap::uiMaxScreenX - 1) || y >= (zMap::uiMaxScreenX - 1)) return false;
		index = (y + 1) * zMap::uiMaxScreenX + (x + 1);
	}
	else if (direct == 4)					//��
	{
		if (y >= (zMap::uiMaxScreenX - 1)) return false;
		index = (y + 1) * zMap::uiMaxScreenX + x;
	}
	else if (direct == 5)					//����
	{
		if (x <= 0 || y >= (zMap::uiMaxScreenX - 1)) return false;
		index = (y + 1) * zMap::uiMaxScreenX + (x - 1);
	}
	else if (direct == 6)					//��
	{
		if (x <= 0) return false;
		index = y * zMap::uiMaxScreenX + (x - 1);
	}
	else if (direct == 7)					//����
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
	//��ʼ����ͼ�������,�͵�ͼ�����
	zMap::oBigWH.x = 2048;
	zMap::oBigWH.y = 2048;
	zMap::uiMaxScreenX = 69;
	zMap::uiMaxScreenY = 69;

	for (uint32 y = 0; y < zMap::uiMaxScreenY; ++y)
	{
		for (uint32 x = 0; x < zMap::uiMaxScreenX; ++x)
		{
			uint32 uiCurIndex = y * zMap::uiMaxScreenX + x;
			//����(ǰ��������)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[0][uiCurIndex];
				uint32 uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//����(ǰ��������)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[1][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//��(ǰ��������)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[2][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//����(ǰ��������)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[3][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//��(ǰ��������)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[4][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//����(ǰ��������)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[5][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//��(ǰ��������)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[6][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//����(ǰ��������)
			{
				std::vector<uint32_t>& vecIndex = direct_screen[7][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}

			//����(����������)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[0][uiCurIndex];
				uint32 uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//����(����������)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[1][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//��(����������)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[2][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//����(����������)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[3][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 6, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//��(����������)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[4][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//����(����������)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[5][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 0, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 7, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//��(����������)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[6][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}
			//����(����������)
			{
				std::vector<uint32_t>& vecIndex = reversedirect_screen[7][uiCurIndex];
				uint32_t uiDirIndex = 0;
				//������
				if (GetDirectIndex(x, y, 5, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 4, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 3, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//����
				if (GetDirectIndex(x, y, 2, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
				//������
				if (GetDirectIndex(x, y, 1, uiDirIndex))
					vecIndex.push_back(uiDirIndex);
			}

			//����
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

//�����������õ���������
const zPosIVector* zScreenIndex::GetNineScreen(const zPosI uiPos)
{
	auto iterFind = nine_screen.find(uiPos);
	if (iterFind == nine_screen.end())
		return nullptr;
	return &(iterFind->second);
}

//�����������ͷ���õ�ǰ��������
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

//�����������ͷ���õ�����������
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
