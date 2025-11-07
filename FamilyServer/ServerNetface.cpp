#include "familysvr.pb.h"
#include "ServerNetface.h"
#include "client.pb.h"
#include "parse_pb.h"
#include "msg_make.h"
#include "global_define.h"
#include "SceneSession.h"
#include "FamilyLogic.h"

#include "FamilyUserMgr.h"

ClientLogic::ClientLogic() { }
ClientLogic::~ClientLogic() { }

bool ClientLogic::netMsgFromScene(DWORD hashId, const tagHostHd& hostHead, inner::InnerFamilysvr innerReq)
{
	Log_Debug("%s,%s",innerReq.GetTypeName().c_str(), innerReq.ShortDebugString().c_str());
	auto fromUser = innerReq.fromuser();
	auto* pUser = gFamilyUserMgr->tryCreateUser(fromUser);
	if (!pUser)
	{
		Log_Error("netMsgFromScene.!pUser.%lu", fromUser);
		return false;
	}
	switch (innerReq.Fromscene_case())
	{
	case inner::InnerFamilysvr::FromsceneCase::kFromsceneGameevent:
	{
		const auto& req = innerReq.fromscene_gameevent();
		switch (req.gameevent().type_case())
		{
		case inner::InnerGameEvent::kLogin: pUser->evGameLogin(req.gameevent()); break;
		}
	}
	break;
	case inner::InnerFamilysvr::FromsceneCase::kFromsceneUpdateuserinfo:
	{
	}
	break;
	default:
		Log_Error("undefined case:%u", innerReq.Fromscene_case());
		break;
	}
	return true;
}