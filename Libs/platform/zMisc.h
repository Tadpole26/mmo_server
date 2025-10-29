#pragma once
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include "zType.h"

namespace zMisc
{
template<typename T>
void listErase(std::list<std::decay_t<T>>& list, T&& unit)
{
	auto it = std::find(list.begin(), list.end(), std::forward<T>(unit));
	if (it != list.end()) list.erase(it);
}
template <typename T>
std::size_t hash(T&& val)
{
	return std::hash<std::decay_t<T>>()(std::forward<T>(val));
}
template<typename T, typename ...ARGS>
std::size_t hash(T&& val, ARGS &&...args)
{
	return std::hash<std::decay_t<T>>()(std::forward<T>(val)) ^ hash(std::forward<ARGS>(args)...);
}
//最小值~最大值(由于windows下rand的数值范围为0-7fff(32767),所以添加此函数)
inline int randMax(int range_max)
{
	if (range_max == 0) return 0;

	return ((((unsigned int)rand() << 24) & 0xFF000000)
		| (((unsigned int)rand() << 12) & 0x00FFF000)
		| (((unsigned int)rand()) & 0x00000FFF)) % range_max;
}

inline int randBetween(int min, int max)
{
	if (min == max)
		return min;
	else
		return ((((unsigned int)rand() << 24) & 0xFF000000)
			| (((unsigned int)rand() << 12) & 0x00FFF000)
			| (((unsigned int)rand()) & 0x00000FFF)) % (max - min) + min;
}

inline double randDBetween(double range_min, double range_max)
{
	return ((double(rand()) / double(RAND_MAX)) * (range_max - range_min)) + range_min;
}
};
