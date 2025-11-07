#pragma once

#include "sdk.h"
#include "cAuction.h"

class AuctionMenu;
class AuctionGoods;
class AuctionObject;

class AuctionFilter
{
public:
	AuctionFilter(uint32 orderType);
	virtual ~AuctionFilter();
public:
	virtual void addObject(AuctionObject& object) = 0;
	virtual void removeObject(AuctionObject &object) = 0;
public:
	cAuction::FilterIdType getHashId() const;				//carceer&pos&level&quality
public:
	void setFilerUnit(uint32 type, cAuction::FilterType val);
protected:
	std::array<cAuction::FilterType, cAuction::FilterType_Size> _filterType;
	uint32 _orderType = cAuction::OrderType_None;
};
