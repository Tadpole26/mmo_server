#pragma once
#include "sdk.h"
#include "cAuction.h"

namespace inner
{
	class InnerAuctionBidPrice;
}

class AuctionObject;
class FamilyUser;
class AuctionBidPrice
{
public:
	AuctionBidPrice();
	~AuctionBidPrice();
public:
	void merge(const AuctionBidPrice &other);
	void merge(const inner::InnerAuctionBidPrice &in);
	void serialize(inner::InnerAuctionBidPrice& out) const {}
	void unserialize(const inner::InnerAuctionBidPrice& in) {}
public:
	uint32 getDiamondTotal() const { return getDiamond() + getDiamondBind(); }
	void clear();
public:
	uint32 getDiamond() const { return _diamond; }
	void setDiamond(uint32 val) { _diamond = val; }
	uint32 getDiamondBind() const { return _diamondBind; }
	void setDiamondBind(uint32 val) { _diamondBind = val; }
private:
	uint32 _diamond = 0;
	uint32 _diamondBind = 0;
};

class AuctionBid
{
public:
	AuctionBid();
	~AuctionBid();
public:
	void fill(common::AuctionUserBid& out) const;
	void serialize(record::AuctionUserBid &out) const;
	void unserialize(const record::AuctionUserBid &in);
public:
	zRoleIdType getRoleId() const;
	uint32 getState() const;
	bool isStateLeading() const;
	bool isStateBeaten() const;
	bool isBidding() const { return _bidId; }
public:
	void evAuctionBidPriceUpdated(uint32 priceOld, uint32 priceNew);
	void evAuctionBidPriceBeaten();
	void evAuctionBidPriceLeading();
	void evAuctionObjectExpired(std::pair<uint32, uint32> *costDetail);
public:
	bool costPrice(uint32 price, std::pair<uint32, uint32> *detail);
public:
	cAuction::ObjectIdType getObjectId() const { return _objectId; }
	const AuctionObject* objectPtr() const { return _object; }
	AuctionObject* objectPtr() { return _object; }
	void setObjectPtr(AuctionObject* object) { _object = object; }
	const FamilyUser* userPtr() const { return _user; }
	FamilyUser* userPtr() { return _user; }
	void setUserPtr(FamilyUser* user) { _user = user; }
	uint32 getBidId() const { return _bidId; }
	void setBidId(uint32 bidId) { _bidId = bidId; }
	const AuctionBidPrice& getPriceBase() const { return _priceBase; }
	AuctionBidPrice& getPriceBase() { return _priceBase; }
	const AuctionBidPrice& getPriceAuto() const { return _priceAuto; }
	AuctionBidPrice& getPriceAuto() { return _priceAuto; }
	uint32 getPriceBaseNum() const { return _priceBase.getDiamondTotal(); }
	uint32 getPriceAutoNum() const { return _priceAuto.getDiamondTotal(); }
	uint32 getPriceNum() const { return getPriceBaseNum() + getPriceAutoNum(); }
	bool isFirstBid() const { return _firstBid; }
	void setFirstBid(bool firstBid) { _firstBid = firstBid; }
private:
	cAuction::ObjectIdType _objectId = 0;
	AuctionObject* _object = nullptr;
	FamilyUser* _user = nullptr;
	uint32 _bidId = 0;
	AuctionBidPrice _priceBase;	//
	AuctionBidPrice _priceAuto;
	bool _firstBid = false;

};