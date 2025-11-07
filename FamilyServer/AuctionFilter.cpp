#include "AuctionFilter.h"

AuctionFilter::AuctionFilter(uint32 orderType) : _orderType(orderType)
{
}

AuctionFilter::~AuctionFilter()
{
}

cAuction::FilterIdType AuctionFilter::getHashId() const
{
	auto filterCareer = _filterType[cAuction::FilterType_Career - 1];
	auto filterPos = _filterType[cAuction::FilterType_Pos - 1];
	auto filterLevel = _filterType[cAuction::FilterType_Level - 1];
	auto filterQuality = _filterType[cAuction::FilterType_Quality - 1];
	return cAuction::unionFilterId(filterCareer, filterPos, filterLevel, filterQuality);
}

void AuctionFilter::setFilerUnit(uint32 type, cAuction::FilterType val)
{
	if (type == cAuction::FilterType_None || type >= cAuction::FilterType_Max)
	{
		assert(false);
		return;
	}
	_filterType[type - 1] = val;
}