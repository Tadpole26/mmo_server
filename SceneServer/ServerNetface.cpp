#include "client.pb.h"
#include "scenesvr.pb.h"

#include "ServerNetface.h"
#include "msg_module_servercommon.pb.h"
#include "parse_pb.h"
#include "msg_make.h"
#include "global_define.h"
#include "CGateSession.h"

ClientLogic::ClientLogic() { }
ClientLogic::~ClientLogic() { }

void CClientLogic::handle_logic_msg(const tagNetMsg* pNetMsg)
{
	const tagHostHd& stHd = pNetMsg->m_hd;
	tagMsgHead* pMsgHead = (tagMsgHead*)(pNetMsg->m_body);
	inner::InnerGatesvr innerReq;
	PARSE_PTL_HEAD(innerReq, pMsgHead);
	zRoleIdType roleId = innerReq.fromuser();
	switch (innerReq.Fromscene_case())
	{
	case inner:kFromgateServerregister:
	{
		CGateSession* pServer = new CGateSession();
		if (!pServer)
		{
			Log_Error("handle_logic_msg.!pServer");
			return;
		}
		add_session(pServer, pNetMsg->m_hd, MAX_SIZE_512M);
		pServer->SetServerId(1);
		Log_Info("handle_logic_msg.svr connect me");
	}
	break;
	case inner::kFromgateCreaterole:
	{
		if (!has_session(pNetMsg->m_hd)) return;
		auto *pGatesvr = gSceneLogic->getGateSvr(1);
		if (!pGatesvr)
		{
			Log_Error("handle_logic_msg.!pServer");
			return;
		}
		pGatesvr->OnCreateRole();
	}
	break;
	case inner::kFromgateClientmsg:
	{
		auto *pGatesvr = gSceneLogic->getGateSvr(1);
		if (!pGatesvr)
		{
			Log_Error("handle_logic_msg.!pServer");
			return;
		}
		pGatesvr->OnCreateRole();
	}
	break;
	default:
	{
		CSvrLogicFace::handle_logic_msg(pNetMsg);
	}
	}
}