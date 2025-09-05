#pragma once
#include "svr_client.h"

namespace inner
{
	//class InnerFriendsvr;
}

class FamilyNetface : public svr_reconn
{
public:
	FamilyNetface();
	~FamilyNetface();

	//bool NetMsgFromFriend(const tagMsgHead* pMsgHead);
	//bool SendCmdToFriendUser(std::function<void(inner::InnerFriendsvr)>);

private:
	virtual void handle_msg(const tagMsgHead* pNetMsg);
	virtual void on_connect();
	virtual void on_disconnect();
	virtual void trigger();
};
