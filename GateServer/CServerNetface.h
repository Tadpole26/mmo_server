#pragma once

#include "server_logic_face.h"


//�����������ӵ��û�
class GateUser;
class CClientLogic : public CSvrLogicFace 
{
public:
	CClientLogic();
	~CClientLogic();

	virtual void handle_logic_msg(const tagNetMsg* pNetMsg);
	//��ͻ������ӶϿ�
	void OnDisconnect(GateUser* pUser);
	void OnKickConnect(GateUser* pUser, bool bSysKill);

private:
	void OnVerifyAccount(const tagMsgHead* pHeadMsg, const tagHostHd& hd);

};
