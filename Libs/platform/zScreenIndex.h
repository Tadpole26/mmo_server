#pragma once
#include "sdk.h"

//地图屏索引
class zScreenIndex
{
public:
	zScreenIndex(const zScreenIndex&) = delete;
	zScreenIndex& operator=(const zScreenIndex&) = delete;
private:
	using nineScreen_map = std::unordered_map<uint32, zPosIVector>;
	using nineScreen_map_iter = nineScreen_map::iterator;
	using nineScreen_map_value_type = nineScreen_map::value_type;
	//九屏索引
	static nineScreen_map nine_screen;
	//前向屏索引(0(下)|1(右下)|2(右)|3(右上)|4(上)|5(左上)|6(左)|7(左下)),上下左右3屏,斜方向5屏
	static nineScreen_map direct_screen[8];
	//后向屏索引(0(下)|1(右下)|2(右)|3(右上)|4(上)|5(左上)|6(左)|7(左下)),上下左右3屏,斜方向5屏
	static nineScreen_map reversedirect_screen[8];
public:
	//根据屏索引得到九屏索引
	const zPosIVector* GetNineScreen(const zPosI uiPos);
	//根据屏索引和方向得到前向屏索引
	const zPosIVector* GetDirectScreen(const zPosI uiPos, const int direct);
	//根据屏索引和方向得到后向屏索引
	const zPosIVector* GetReverseDirectScreen(const zPosI uiPos, const int direct);
public:
	//初始化屏关系
	static bool Init();
	//构造
	zScreenIndex();
	//析构
	~zScreenIndex();
};