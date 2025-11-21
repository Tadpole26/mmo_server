#include "AuctionBid.h"

AuctionBidPrice::AuctionBidPrice()
{
}

AuctionBidPrice::~AuctionBidPrice()
{
}

void AuctionBidPrice::merge(const AuctionBidPrice& other)
{
	setDiamond(getDiamond() + other.getDiamond());
	setDiamondBind(getDiamondBind() +other.getDiamondBind());
}

void AuctionBidPrice::merge(const inner::InnerAuctionBidPrice& in)
{
	setDiamond(getDiamond() + in.diamond());
	setDiamondBind(getDiamondBind() + in.diamondbind());
}

void AuctionBidPrice::clear()
{
	setDiamond(0);
	setDiamondBind(0);
}

AuctionBid::AuctionBid()
{
}

//AuctionBid::~AuctionBid();
//zRoleIdType getRoleId() const;
//uint32 getState() const;
//bool isStateLeading() const;
//bool isStateBeaten() const;
//void evAuctionBidPriceUpdated(uint32 priceOld, uint32 priceNew);
//void evAuctionBidPriceBeaten();
//void evAuctionBidPriceLeading();
//void evAuctionObjectExpired(std::pair<uint32, uint32>* costDetail);
//bool costPrice(uint32 price, std::pair<uint32, uint32>* detail);