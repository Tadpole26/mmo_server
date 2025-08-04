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
	//���ضϿ����ӻص�
	virtual void on_disconnect();
	void SendLoginErrorRet(int64_t llUid, uint32 uiSeqId, uint32 eCode);
private:
	void OnCreateRole(uchar* pMsg, uint32 uiLen);
public:
	bool handClientMsg(const InnerGatesvr& innerReq);
};
