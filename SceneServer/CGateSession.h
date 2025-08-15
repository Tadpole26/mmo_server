#pragma once
#include "sdk.h"
#include "logic_interface.h"
#include "svr_client.h"
#include "server_pool.h"

namespace inner
{
	class InnerScenesvr;
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
public:
	bool handClientMsg(const inner::InnerScenesvr& innerReq);
	void OnCreateRole(const inner::InnerScenesvr& innerReq);
};
