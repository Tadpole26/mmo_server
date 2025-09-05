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

bool ClientLogic::netMsgFromGate(DWORD hashId, const tagHostHd& hostHead, inner::InnerScenesvr innerReq)
{
	switch (innerReq.Fromgate_case())
	{
	case inner::InnerScenesvr::FromgateCase::kFromgateServerregister:
		{
			CGateSession* pServer = new CGateSession();
			if (!pServer)
			{
				Log_Error("handle_logic_msg.!pServer");
				return false;
			}
			add_session(pServer, hostHead, MAX_SIZE_512M);
			pServer->SetServerId(hashId);
			Log_Info("handle_logic_msg.svr connect me");
		}
		break;
	case inner::InnerScenesvr::FromgateCase::kFromgateCreaterole:
		{
			if (!has_session(hostHead)) return false;
			auto* pGatesvr = gSceneLogic->getGateSvr(1);
			if (!pGatesvr)
			{
				Log_Error("handle_logic_msg.!pServer");
				return false;
			}
			pGatesvr->OnCreateRole(innerReq);
		}
		break;
	case inner::InnerScenesvr::FromgateCase::kFromgateClientmsg:
		{
			auto* pGatesvr = gSceneLogic->getGateSvr(0);
			if (!pGatesvr)
			{
				Log_Error("handle_logic_msg.!pServer");
				return false;
			}
			pGatesvr->handClientMsg(innerReq);
		}
		break;
	default:
		Log_Error("netMsgFromGate.%s", innerReq.ShortDebugString().c_str());
		break;
	}
	return true;
}

bool ClientLogic::netMsgFromFriend(DWORD hashId, const tagHostHd& hostHead, inner::InnerScenesvr innerReq)
{
	return true;
}