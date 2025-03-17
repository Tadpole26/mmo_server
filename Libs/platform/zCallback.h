#pragma once
//回调函数类模版

template <typename T, typename RTValue = bool>
struct zCallback
{
	virtual RTValue exec(T* e) = 0;
	virtual ~zCallback() {}
};

//删除条件接口
template <typename T>
struct zRemoveIf
{
	//被删除的entity存储在这里
	std::vector<T*> removed;
	virtual bool isIt(T* entity) = 0;
	virtual ~zRemoveIf() {}
};
