#include "familysvr.pb.h"
#include "ServerNetface.h"
#include "client.pb.h"
#include "parse_pb.h"
#include "msg_make.h"
#include "zLogMgr.h"
#include "global_define.h"
#include "SceneSession.h"
#include "FamilyLogic.h"

ClientLogic::ClientLogic() { }
ClientLogic::~ClientLogic() { }

void ClientLogic::handle_logic_msg(const tagNetMsg* pNetMsg)
{
	const tagHostHd& stHd = pNetMsg->m_hd;
	tagMsgHead* pMsgHead = (tagMsgHead*)(pNetMsg->m_body);
	inner::InnerFamilysvr innerReq;
	PARSE_PTL_HEAD(innerReq, pMsgHead);
	zRoleIdType roleId = innerReq.fromuser();
	switch (innerReq.Fromscene_case())
	{
	case inner::InnerFamilysvr::FromsceneCase::kFromsceneGameevent:
	{
		auto* pScenesvr = gFamilyLogic->getScene(1);
		if (!pScenesvr)
		{
			Log_Error("handle_logic_msg.!pScenesvr");
			return;
		}
	}
	break;
	case inner::InnerFamilysvr::FromsceneCase::kFromsceneUpdateuserinfo:
	{
		auto* pScenesvr = gFamilyLogic->getScene(1);
		if (!pScenesvr)
		{
			Log_Error("handle_logic_msg.!pScenesvr");
			return;
		}
	}
	break;
	default:
	{
		CSvrLogicFace::handle_logic_msg(pNetMsg);
	}
	}
}