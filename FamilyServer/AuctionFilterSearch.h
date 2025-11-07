#pragma once

#include "AuctionFilter.h"
#include "skiplist.h"

class AuctionFilterSearch : public AuctionFilter
{
public:
	AuctionFilterSearch(uint32 orderType);
	~AuctionFilterSearch();
public:
	void addObject(AuctionObject &object) override;
	void removeObject(AuctionObject &object) override;
private:
	std::unordered_map<cAuction::ObjectIdType, CTCSkipList<AuctionObject*>::const_iterator> _objects;
	std::map<uint32, CTCSkipList<AuctionObject*>> _pirceIndex; //key:价格区间段
};