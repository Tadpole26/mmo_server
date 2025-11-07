#include "AuctionFilterSearch.h"
#include "AuctionObject.h"

AuctionFilterSearch::AuctionFilterSearch(uint32 orderType) : AuctionFilter(orderType)
{
}

AuctionFilterSearch::~AuctionFilterSearch()
{
}

void AuctionFilterSearch::addObject(AuctionObject& object)
{
	uint32 score = 0;
	switch (_orderType)
	{
	case cAuction::OrderType_Time: score = object.getBidExpire(); break;
	case cAuction::OrderType_Price:score = object.getPriceShown(); break;
	default:
		Log_Error("addObject.!type,%u", _orderType);
		return;
	}
	auto& priceIndex = _priceIndex[object.getPriceRank()];
	auto* node = priceIndex.Insert(&object);
	if (node == priceIndex.End())
	{
		Log_Error("addObject.!node.%lu", object.getObjectId());
		return;
	}
	if (!_objects.emplace(object.getObjectId(), node).second)
	{
		priceIndex.Remove(&object);
		return;
	}
}

void AuctionFilterSearch::removeObject(AuctionObject& object)
{
	auto it = _objects.find(object.getObjectId());
	if (it == _objects.end())
	{
		Log_Error("removeObject.end,%lu", object.getObjectId());
		return;
	}
	const auto* node = it->second;
	_objects.erase(it);
	auto& priceIndex = _pirceIndex[object.getPriceRank()];
	priceIndex.Remove(node->data);
	if (!priceIndex.length())
	{
		_priceIndex.erase(object.getPriceRank());
	}
}