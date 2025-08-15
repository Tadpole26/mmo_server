#include "client.pb.h"
#include "scenesvr.pb.h"

#include "ServerNetface.h"
#include "SceneLogic.h"
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
	inner::InnerScenesvr innerReq;
	PARSE_PTL_HEAD(innerReq, pMsgHead);
	zRoleIdType roleId = innerReq.fromuser();
	switch (innerReq.Fromgate_case())
	{
	case inner::InnerScenesvr::FromgateCase::kFromgateServerregister:
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
	case inner::InnerScenesvr::FromgateCase::kFromgateCreaterole:
	{
		if (!has_session(pNetMsg->m_hd)) return;
		auto *pGatesvr = gSceneLogic->getGateSvr(1);
		if (!pGatesvr)
		{
			Log_Error("handle_logic_msg.!pServer");
			return;
		}
		pGatesvr->OnCreateRole(innerReq);
	}
	break;
	case inner::InnerScenesvr::FromgateCase::kFromgateClientmsg:
	{
		auto *pGatesvr = gSceneLogic->getGateSvr(1);
		if (!pGatesvr)
		{
			Log_Error("handle_logic_msg.!pServer");
			return;
		}
		pGatesvr->handClientMsg(innerReq);
	}
	break;
	default:
	{
		CSvrLogicFace::handle_logic_msg(pNetMsg);
	}
	}
}