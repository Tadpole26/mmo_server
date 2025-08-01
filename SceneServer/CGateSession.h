#pragma once
#include "sdk.h"
#include "logic_interface.h"
#include "svr_client.h"
#include "server_pool.h"

namespace inner
{
	class InnerGatesvr;
}

class CGateSession : public svr_session
{
public:
	CGateSession();
	~CGateSession();

	virtual void handle_msg(const tagMsgHead* pNetMsg);
	//网关断开连接回调
	virtual void on_disconnect();
	void SendLoginErrorRet(int64_t llUid, uint32 uiSeqId, uint32 eCode);

private:
	void OnAccountEnter(uchar* pMsg, uint32 uiLen);
	void OnCreatePlayer(uchar* pMsg, uint32 uiLen);
	void OnSavePlayer(uchar* pMsg, uint32 uiLen);
public:
	bool netMsgFromGate(const tagMsgHead* pNetMsg);
};
