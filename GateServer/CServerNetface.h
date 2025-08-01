#pragma once

#include "server_logic_face.h"


//管理所有连接的用户
class GateUser;
class CClientLogic : public CSvrLogicFace 
{
public:
	CClientLogic();
	~CClientLogic();

	virtual void handle_logic_msg(const tagNetMsg* pNetMsg);
	//与客户端连接断开
	void OnDisconnect(GateUser* pUser);
	void OnKickConnect(GateUser* pUser, bool bSysKill);

private:
	void OnVerifyAccount(const tagMsgHead* pHeadMsg, const tagHostHd& hd);

};
