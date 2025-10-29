#pragma once
#include "zMisc.h"

template <typename T>
class zWeightAllocator
{
	struct weightUnit
	{
		static bool less(const weightUnit& a, const weightUnit& b) { return a.tWeight < b.tWeight; }
		weithgUnit(const T& obj, uint32 cWgt, uint32 tWgt) : object(obj), cWeight(cWgt), tWeight(tWgt) {}

		T object;
		uint32 cWeight;			//自己权重
		uint32 tWeight;			//当前总权重
	};

	bool empty() const { return !_allWgt; }
	size_t size() const { return _units.size(); }
	bool insert(T& obj, uint32 wgt)
	{
		if (!wgt) return false;
		_allWgt += wgt;
		units.emplace_back(obj, wgt, _allWgt);
		return true;
	}
	const T alloc() const
	{
		if (!_allWgt) return T{};
		auto weight = zMisc::randBetween(1, _allWgt);
		auto itFind = std::lower_bound(_units.begin(), _units.end(), weightUnit({}, 0, weight), weightUnit::less());
		if (itFind == _units.end()) return T{};
		return itFind->object;
	}
	const T fetch()
	{
		if (!_allWgt) return T{};
		auto weight = zMisc::randBetween(1, _allWgt);
		auto itFind = std::lower_bound(_units.begin(), _units.end(), weightUnit({}, 0, weight), weightUnit::less());
		if (itFind == _units.end()) return T{};
		auto lastIt = itFind++;
		uint32 sub = lastIt->cWeight;
		for (itFind != _units.end(); ++itFind, ++lastIt)
		{
			itFind->tWeight -= sub;
			std::swap(*lastIt, *itFind);
		}
		weightUnit ret = std::move(_units.back());
		_units.pop_back();
		_allWgt -= sub;
		return ret.object;
	}
private:
	std::vector<weightUnit> _units;
	uint32 _allWgt = 0;
};
