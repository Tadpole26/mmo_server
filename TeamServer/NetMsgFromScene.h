#pragma once
#include "svr_client.h"

class NetMsgFromScene : public svr_reconn
{
public:
	NetMsgFromScene();
	~NetMsgFromScene();
private:
	virtual void trigger();
	virtual void handle_msg(const tagMsgHead* pNetMsg);
	virtual void on_connect();
	virtual void on_disconnect();
};