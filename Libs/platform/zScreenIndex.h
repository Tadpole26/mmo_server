#pragma once
#include "sdk.h"

//��ͼ������
class zScreenIndex
{
public:
	zScreenIndex(const zScreenIndex&) = delete;
	zScreenIndex& operator=(const zScreenIndex&) = delete;
private:
	using nineScreen_map = std::unordered_map<uint32, zPosIVector>;
	using nineScreen_map_iter = nineScreen_map::iterator;
	using nineScreen_map_value_type = nineScreen_map::value_type;
	//��������
	static nineScreen_map nine_screen;
	//ǰ��������(0(��)|1(����)|2(��)|3(����)|4(��)|5(����)|6(��)|7(����)),��������3��,б����5��
	static nineScreen_map direct_screen[8];
	//����������(0(��)|1(����)|2(��)|3(����)|4(��)|5(����)|6(��)|7(����)),��������3��,б����5��
	static nineScreen_map reversedirect_screen[8];
public:
	//�����������õ���������
	const zPosIVector* GetNineScreen(const zPosI uiPos);
	//�����������ͷ���õ�ǰ��������
	const zPosIVector* GetDirectScreen(const zPosI uiPos, const int direct);
	//�����������ͷ���õ�����������
	const zPosIVector* GetReverseDirectScreen(const zPosI uiPos, const int direct);
public:
	//��ʼ������ϵ
	static bool Init();
	//����
	zScreenIndex();
	//����
	~zScreenIndex();
};