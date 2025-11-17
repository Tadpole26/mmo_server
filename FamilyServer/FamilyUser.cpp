#include "FamilyUser.h"

FamilyUser::FamilyUser(zRoleIdType roleId) : cUser(roleId)
{
}

FamilyUser::~FamilyUser()
{
}

bool FamilyUser::init()
{
	return true;
}

void FamilyUser::final()
{
}

void FamilyUser::evGameLogin(const inner::InnerGameEvent& in)
{
	Log_Info("evGameLogin");
}

void FamilyUser::evGameLogout(const inner::InnerGameEvent& in)
{
	Log_Info("evGameLogout");
}

void FamilyUser::evGameOnline(const inner::InnerGameEvent& in)
{
	Log_Info("evGameOnline");
}

void FamilyUser::evGameOffline(const inner::InnerGameEvent& in)
{
	Log_Info("evGameOffline");
}

void FamilyUser::evGameChangeMap(const inner::InnerGameEvent& in)
{
	Log_Info("evGameChangeMap");
}

void FamilyUser::evGameWakeup(const inner::InnerGameEvent& in)
{
	Log_Info("evGameWakeup");
}

bool FamilyUser::innerOperationReq(const inner::InnerAuctionOperationReq& op)
{
	if (op.seq() <= _seqRecv)
	{
		return false;
	}
	_seqRecv = op.seq();
	saveDelay();
	switch (op.op_case())
	{
	case inner::InnerAuctionOperationReq::OpCase::kGoodsup: innerGoodsUp(op.goodsup()); break;
	case inner::InnerAuctionOperationReq::OpCase::kGoodsbid: innerGoodBid(op.goodsbid()); break;
	}
}

bool FamilyUser::innerOperationReq(consg inner::InnerAuctionOperatinNtf& op)
{
	removeResCache(op.seq());
	return true;
}

void FamilyUser::removeResCache(uint64 seq)
{
	for (auto it = _resCache.begin(); it != _resCache.end();)
	{
		auto del = it++;
		auto* cache = *del;
		if (seq == cache->seq())
		{
			SAFE_DELETE(cache);
			_resCache.erase(del);
			saveDelay();
			break;
		}
		if (seq < cache->seq())
		{
			break;
		}
		Log_Error("disorder");
		assert(false);
	}
}

bool FamilyUser::innerGoodsUp(const inner::InnerAuctionGoodsUp& goodsUp)
{
	inner::InnerAuctionOperationRes res;
	res.set_seq(_seqRecv);
	zDefer defer([&] 
		{
			addResCache(std::move(res));
		});
	//error handle 

	res.set_success(true);
}