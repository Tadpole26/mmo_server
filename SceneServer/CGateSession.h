#pragma once
#include "logic_interface.h"
#include "svr_client.h"
#include "server_pool.h"
using namespace ProtoMsg;

class CGateSession : public svr_session
{
public:
	CGateSession();
	~CGateSession();

	virtual void handle_msg(const tagMsgHead* pNetMsg);
	//���ضϿ����ӻص�
	virtual void on_disconnect();

	void HandleTransmitData(const tagMsgHead* pNetMsg);
	void SendLoginErrorRet(int64_t llUid, uint32 uiSeqId, ResultCode eCode);

private:
	void OnAccountEnter(uchar* pMsg, uint32 uiLen);
	void OnCreatePlayer(uchar* pMsg, uint32 uiLen);
	void OnSavePlayer(uchar* pMsg, uint32 uiLen);
};
