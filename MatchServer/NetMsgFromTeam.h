#pragma once
#include "sdk.h"
#include "svr_client.h"

class NetMsgFromTeam : public svr_reconn
{
public:
	NetMsgFromTeam();
	~NetMsgFromTeam();
private:
	virtual void trigger();
	virtual void handle_msg(const tagMsgHead* pNetMsg);
	virtual void on_connect();
	virtual void on_disconnect();
};