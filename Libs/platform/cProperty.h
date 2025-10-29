#pragma once
#include "sdk.h"

enum eProperty
{
	eProperty_None = 0,//默认
	eProperty_Str = 1,//力量 
	eProperty_Agi = 2,//敏捷
	eProperty_Vit = 3,//体质
	eProperty_Int = 4,//智力
	eProperty_Dex = 5,//灵巧
	eProperty_Luk = 6,//幸运
	eProperty_MAX,
};

class cProperty
{
public:
	cProperty() {}
	virtual ~cProperty() {}
	virtual bool valid(uint32 prop) const { return false; }
	virtual void fix(uint32 prop) {}

	uint32 get(uint32 prop) const
	{
		auto it = _attrmap.find(prop);
		return it == _attrmap.end() ? 0 : it->second;
	}
	void set(uint32 prop, uint32 val)
	{
		if (val == 0)
		{
			auto it = _attrmap.find(prop);
			if (it == _attrmap.end()) return;
			_attrmap.erase(prop);
		}
		else
		{
			auto &current = _attrmap[prop];
			if (current == val) return;
			current = val;
		}
		fix(prop);
		_update = true;
	}
	void inc(uint32 prop, uint32 val)
	{
		if (val == 0) return;
		_attrmap[prop] += val;
		fix(prop);
		_update = true;
	}
	void sub(uint32 prop, uint32 val)
	{
		if (val == 0) return;
		auto it = _attrmap.find(prop);
		if (it == _attrmap.end()) return;
		if (it->second > val) it->second -= val;
		else _attrmap.erase(prop);
		fix(prop);
		_update = true;
	}
	bool empty() const
	{
		return _attrmap.empty();
	}
	void clear()
	{
		_attrmap.clear();
	}
	cProperty& operator = (const cProperty& other)
	{
		if (this != &other)
		{
			_attrmap = other._attrmap;
			_update = true;
		}
		return *this;
	}
	cProperty& operator += (const cProperty & other)
	{
		if (this != &other)
		{
			for (const auto& pair : other._attrmap)
			{
				_attrmap[pair.first] += pair.second;
			}
			_update = true;
		}
		return *this;
	}
private:
	std::map<uint32, uint32> _attrmap;
	bool _update = false;					//是否更新成功
};

class cBasePropety : public cProperty
{
public:
	bool valid(uint32 prop) const override;
};