#include "CServerNetface.h"
#include "msg_module_servercommon.pb.h"
#include "module.pb.h"
#include "parse_pb.h"
#include "msg_make.h"
#include "log_mgr.h"
#include "global_define.h"

CClientLogic::CClientLogic() { }
CClientLogic::~CClientLogic() { }

void CClientLogic::handle_logic_msg(const tagNetMsg* pNetMsg)
{
	const tagHostHd& stHd = pNetMsg->m_hd;
	tagMsgHead* pMsgHead = (tagMsgHead*)(pNetMsg->m_body);

	switch (pMsgHead->usModuleId)
	{
	case 1: break;
	default: CSvrLogicFace::handle_logic_msg(pNetMsg); break;
	}
}