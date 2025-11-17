#pragma once
#include "sdk.h"
#include "cUser.h"

namespace inner
{
	class InnerGameEvent;
}

class FamilyUser : public cUser
{
public:
	FamilyUser(zRoleIdType roleId);
	~FamilyUser();
	bool init();
	void final();
public:
	void tick() {}
public:
	void evGameLogin(const inner::InnerGameEvent &in);
	void evGameLogout(const inner::InnerGameEvent &in);
	void evGameOnline(const inner::InnerGameEvent& in);
	void evGameOffline(const inner::InnerGameEvent& in);
	void evGameChangeMap(const inner::InnerGameEvent& in);
	void evGameWakeup(const inner::InnerGameEvent& in);
public:
	uint32 getTickGroup() { return _tickGroup; }
public:
	void setTickGroup(uint32 val) { _tickGroup = val; }
public:
	bool innerOperationReq(const inner::InnerAuctionOperationReq &op);
	bool innerOperationReq(consg inner::InnerAuctionOperatinNtf &op);

	bool innerGoodsUp(const inner::InnerAuctionGoodsUp &goodsUp);
private:
	uint32 _tickGroup = 0;
	uint64 _seqRecv = 0;
	std::list<inner::InnerAuctionOperationRes*> _resCache;
};
