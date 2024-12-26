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
};
