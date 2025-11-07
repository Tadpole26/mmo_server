#include "AuctionFilterMenu.h"

#include "AuctionObject.h"

AuctionFilterMenu::AuctionFilterMenu(uint32 orderType) : AuctionFilter(orderType)
{
}

AuctionFilterMenu::~AuctionFilterMenu()
{
}

void AuctionFilterMenu::addObject(AuctionObject& object)
{
	uint32 score = 0;
	switch (_orderType)
	{
	case cAuction::OrderType_Time: score = object.getBidExpire(); break;
	case cAuction::OrderType_Price: score = object.getPriceShown(); break;
	default:
		Log_Error("addObject.!type£¬%u", _orderType);
		return;
	}
	auto data = std::make_pair(object->getObjectId(), object);
	auto ret = _objects.Insert(data, score);
	if (!ret.second)
	{
		Log_Error("addobject.!ret.second.%lu", object->getObjectId());
	}
}

void AuctionFilterMenu::removeObject(AuctionObject& object)
{
	_objects.Erase(object->getObjectId());
}