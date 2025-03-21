#pragma once
#include <vector>

template<typename T>
class vector_to_initializer_list
{
public:
	vector_to_initializer_list(const std::vector<T>& vec)
	{
		_array = vec.data();
		_len = vec.size();
	}
	operator std::initializer_list<T>()
	{
		return *(std::initializer_list<T> *)(this);
	}
private:
	typename std::initializer_list<T>::iterator _array = nullptr;
	typename std::initializer_list<T>::size_type _len = 0;
};