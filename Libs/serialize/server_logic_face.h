#pragma once
#include "../network/logic_interface.h"
#include "msg_make.h"

namespace inner
{
	class InnerScenesvr;
}

struct tagMsgHead;
class CSvrLogicFace : public CLogicInterface, public CMsgMake
{
public:
	CSvrLogicFace() { }
	~CSvrLogicFace() { }
public:
	virtual void handle_logic_msg(const tagNetMsg* pNetMsg);
private:
	bool __msgParseScenesvr(DWORD hashId, const tagHostHd& hostHead, inner::InnerScenesvr innerReq);
public:
	virtual bool netMsgFromGate(DWORD hashId, const tagHostHd& hostHead, inner::InnerScenesvr innerReq);
	virtual bool netMsgFromFriend(DWORD hashId, const tagHostHd& hostHead, inner::InnerScenesvr innerReq);
};
