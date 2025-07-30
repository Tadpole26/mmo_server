#include "ServerNetface.h"
#include "msg_module_servercommon.pb.h"
#include "module.pb.h"
#include "parse_pb.h"
#include "msg_make.h"
#include "global_define.h"
#include "CGateSession.h"

ClientLogic::ClientLogic() { }
ClientLogic::~ClientLogic() { }

void ClientLogic::handle_logic_msg(const tagNetMsg* pNetMsg)
{
	const tagHostHd& stHd = pNetMsg->m_hd;
	tagMsgHead* pMsgHead = (tagMsgHead*)(pNetMsg->m_body);

	switch (pMsgHead->usModuleId)
	{
	default:
	{
		CSvrLogicFace::handle_logic_msg(pNetMsg);
	}
		break;
	}
}