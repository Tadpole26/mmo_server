#pragma once
#include "sdk.h"

namespace cAuction
{
	using FilterIdType = uint64;
	using FilterType = uint32;
	using FilterPair = std::pair<uint32, uint32>;
	using Order = std::pair<uint32, bool>;
	enum
	{
		FilterType_None = 0,
		FilterType_Career = 1,
		FilterType_Pos = 2,
		FilterType_Level = 3,
		FilterType_Quality = 4,
		FilterType_Max,
		FilterType_Size = FilterType_Max - 1,
	};
	enum
	{
		OrderType_None = 0,
		OrderType_Time = 1,
		OrderType_Price = 2,
		OrderType_Max,
		OrderType_Size = OrderType_Max - 1,
	};

	inline bool isValidOrderType(uint32 orderType) { return orderType > OrderType_None && orderType < OrderType_Max; }
	constexpr uint32 filterIdBitsReserved = 30;//保留30位,后面扩展
	constexpr uint32 filterIdBitsCareer = 17;
	constexpr uint32 filterIdBitsPos = 4;
	constexpr uint32 filterIdBitsLevel = 10;
	constexpr uint32 filterIdBitsQuality = 3;

	inline FilterIdType unionFilterId(uint32 career, uint32 pos, uint32 level, uint32 quality)
	{
		assert(career < (1 << filterIdBitsCareer));
		assert(pos < (1 << filterIdBitsPos));
		assert(level < (1 << filterIdBitsLevel));
		assert(quality < (1 << filterIdBitsQuality));
		return FilterIdType(career) << (filterIdBitsPos + filterIdBitsLevel + filterIdBitsQuality) + (pos << (filterIdBitsLevel + filterIdBitsQuality)) + (level << filterIdBitsQuality) + quality;
	}
	inline uint32 splitFilterIdCareer(FilterIdType filterId)
	{
		return (filterId << filterIdBitsReserved) >> (filterIdBitsReserved + filterIdBitsPos + filterIdBitsLevel + filterIdBitsQuality);
	}
	inline uint32 splitFilterIdPos(FilterIdType filterId)
	{
		return (filterId << (filterIdBitsReserved + filterIdBitsCareer)) >> (filterIdBitsReserved + filterIdBitsCareer + filterIdBitsLevel + filterIdBitsQuality);
	}
	inline uint32 splitFilterIdLevel(FilterIdType filterId)
	{
		return (filterId << (filterIdBitsReserved + filterIdBitsCareer + filterIdBitsPos)) >> (filterIdBitsReserved + filterIdBitsCareer + filterIdBitsPos + filterIdBitsQuality);
	}
	inline uint32 splitFilterIdQuality(FilterIdType filterId)
	{
		return (filterId << (sizeof(FilterIdType) * 8 - filterIdBitsQuality)) >> (sizeof(FilterIdType) * 8 - filterIdBitsQuality);
	}

	using ObjectIdType = uint64;
}