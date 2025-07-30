#pragma once
#include "logic_interface.h"
#include "msg_interface.h"
#include "msg_parser.h"
#include "server_logic_face.h"


//�����������������ķ���
class ClientLogic : public CSvrLogicFace
{
public:
	ClientLogic(void);
	~ClientLogic(void);
public:
	virtual void handle_logic_msg(const tagNetMsg* pNetMsg);
private:
	void HandleServerCommon(tagMsgHead* pMsgHead, const tagHostHd& hd);
	void OnRegisterServer(tagMsgHead* pMsgHead, const tagHostHd& hd);
};
