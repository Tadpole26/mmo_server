#pragma once
#include "svr_client.h"

class NetMsgFromMatch : public svr_reconn
{
public:
	NetMsgFromMatch();
	~NetMsgFromMatch();
private:
	virtual void trigger();
	virtual void handle_msg(const tagMsgHead* pNetMsg);
	virtual void on_connect();
	virtual void on_disconnect();
};
