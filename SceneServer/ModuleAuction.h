#pragma once
#include "GameModule.h"

namespace inner
{
	class InnerAuctionOperationReq;
	class InnerAuctionOperationRes;
}

class ModuleAuction : public GameModule
{
public:
	using opSucceedHandleT = std::function<void(const inner::InnerAuctionOperationRes&)>;
	using opFailedHandleT = std::function<void(const inner::InnerAuctionOperationReq&)>;
public:
	bool innerOperationRes(const inner::InnerAuctionOperationRes &op);
public:
	uint64 allocSeq();
	void addReqCache(const inner::InnerAuctionOperationReq& op);
	void removeReqCache(const inner::InnerAuctionOperationRes &op, opSucceedHandleT&& handleSucceed, opFailedHandleT&& handleFailed);
	void resetdAllReqCache();
	void operationFailed(const inner::InnerAuctionOperationReq &op);
	void operationSucceed(const inner::InnerAuctionOperationRes& op);
private:
	uint32 _seqPartTime = 0;
	uint32 _seqPartAuto = 0;
	std::list<inner::InnerAuctionOperationReq*> _reqCache;
	uint64 _seqSend = 0;
};