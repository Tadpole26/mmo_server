#include "ModuleAuction.h"

uint64 ModuleAuction::allocSeq()
{
	auto curTime = GetCurrTime();
	if (curTime >> 3 != _seqPartTime)
	{
		_seqPartTime = (curTime >> 3);
		_seqPartAuto = 0;
	}
	if (_seqPartAuto >= (1 << 4))
	{
		return 0;
	}
	return ((uint64)_seqPartTime << 4) + (_seqPartAuto++);
}

void ModuleAuction::addReqCache(const inner::InnerAuctionOperationReq& op)
{
	assert(op.seq());
	auto* cache = new inner::InnerAuctionOperationReq(op);
	_reqCache.push_back(cache);
	saveNow();
	gameUserRef().msgsendCmptRef().sendCmdToAuctionUserBy([&](auto& out)
		{
			auto& innerReq = *out.mutable_fromscene_operationreq();
			*innerReq.mutable_req() = std::move(op);
		});
}

void ModuleAuction::resetdAllReqCache()
{
	//循环请求列表_reqCache
	gameUserRef().msgsendCmptRef().sendCmtToAuctionUserBy([&](auto& out)
		{
			auto& innerReq = *out.mutable_fromscene_operationreq();
			*innerReq.mutable_req() = op;
		});
	return;
}

void ModuleAuction::removeReqCache(const inner::InnerAuctionOperationRes& op, opSucceedHandleT& handleSucceed, opFailedHandleT& handleFailed)
{
	for (auto it = _reqCache.begin(); it != _reqCache.end();)
	{
		auto del = it++;
		auto* cache = *del;
		if (op.seq() == cache->seq())
		{
			if (op.success())
			{
				if (handleSucceed) handleSucceed(op);
			}
			else
			{
				if (handleFailed) handleFailed(*cache);
			}
			SAFE_DELETE(cache);
			_reqCache.erase(del);
			break;
		}
		if (op.seq() < cache->seq())
		{
			break;
		}
		Log_Error("%s,%s",op.ShortDebugString().c_str(), cache->ShortDebugString().c_str());
		assert(false);
	}
}

bool ModuleAuction::innerOperationRes(const inner::InnerAuctionOperationRes& op)
{
	if (op.seq() <= _seqSend)
	{
		Log_WARN("!seq");
	}
	else
	{
		_seqSend = op.seq();
		removeReqCache(op,
			[&](const auto& res)
			{
				operationSucceed(res);
			},
			[&](const auto& res)
			{
				operationFailed(req);
			}, );
		saveDelay();
	}
	gameUserRef().msgsendCmptRef().sendCmdToAuctionUserBy([&](auto& out)
		{
			auto& innerReq = *out.mutable_fromsceen_operationntf();
			auto& ntf = *innerReq.mutable_ntf();
			ntf.set_seq(op.seq());
		});
	return true;
}