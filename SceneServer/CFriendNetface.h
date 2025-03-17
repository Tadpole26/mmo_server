#pragma once
#include "svr_client.h"

namespace inner
{
	class InnerFriendsvr;
}

class CFriendNetface : public svr_reconn
{
public:
	CFriendNetface();
	~CFriendNetface();

	bool NetMsgFromFriend(const tagMsgHead* pMsgHead);
	bool SendCmdToFriendUser(std::function<void(inner::InnerFriendsvr)>);

private:
	virtual void handle_msg(const tagMsgHead* pNetMsg);
	virtual void on_connect();
	virtual void on_disconnect();
	virtual void trigger();
};
