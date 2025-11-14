#pragma once
#include <functional>

class zDefer
{
public:
	using DeferFuncT = std::function<void()>;
private:
	DeferFuncT _f;
public:
	zDefer(DeferFuncT&& f) : _f(std::move(f))
	{
	}
	~zDefer()
	{
		if (_f)
		{
			_f();
		}
	}
};
