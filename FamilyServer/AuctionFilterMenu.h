#pragma once

#include "AuctionFilter.h"
#include "rank_map.h"

class AuctionMenu;
class AuctionGoods;
class AuctionObject;

class AuctionFilterMenu final : public AuctionFilter
{
public:
	AuctionFilterMenu(uint32 orderType);
	~AuctionFilterMenu();
public:
	void addObject(AuctionObject& object) override;
	void removeObject(AuctionObject &object) override;
private:
	RankMap<cAuction::ObjectIdType, AuctionObject*> _objects;
};
