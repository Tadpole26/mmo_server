#pragma once
#include <stdio.h>
#include <vector>

template <typename T>
inline void SimpleRemoveFromVec(std::vector<T>& vec, T val)
{
	for (typename std::vector<T>::iterator iter = vec.begin(); iter != vec.end(); ++iter)
	{
		if (*iter == val)
		{
			*iter = vec.back();
			vec.erase(vec.end()-1);
			return;
		}
	}
}
