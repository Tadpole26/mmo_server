#pragma once
#include "sdk.h"
#include "logic_interface.h"
#include "svr_client.h"
#include "server_pool.h"

namespace inner
{
	class InnerFamilysvr;
}

class SceneSession : public svr_session
{
public:
	SceneSession();
	~SceneSession();

	virtual void handle_msg(const tagMsgHead* pNetMsg);
	//���ضϿ����ӻص�
	virtual void on_disconnect();
public:
	bool handClientMsg(const inner::InnerScenesvr& innerReq);
	void OnCreateRole(const inner::InnerScenesvr& innerReq);
};

