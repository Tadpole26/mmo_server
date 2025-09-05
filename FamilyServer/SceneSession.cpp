#include "familysvr.pb.h"

#include "SceneSession.h"
#include "parse_pb.h"

SceneSession::SceneSession()
{
	SetServerKind(SERVER_KIND_SCENE);
	SetServerId(0);
}

SceneSession::~SceneSession()
{
}

void SceneSession::on_disconnect()
{
	Log_Warning("GateSession %u disconnect!", m_uiServerId);
}

void SceneSession::handle_msg(const tagMsgHead* pNetMsg)
{
	Log_Info("handle_msg");
	return;
}

bool CGateSession::handClientMsg(const inner::InnerScenesvr& innerReq)
{
	zRoleIdType roleId = innerReq.fromuser();
	auto& clientMsg = innerReq.fromgate_clientmsg();
	auto* pUser = gGameUserMgrIns->getUser(roleId);
	if (!pUser)
	{
		Log_Error("user id %lu not find!moduleid:%u,cmdid:%u", roleId, clientMsg.moduleid(), clientMsg.cmdid());
		return false;
	}
	auto* pModule = pUser->getModule(clientMsg.moduleid());
	if (!pModule)
	{
		Log_Error("moduleid not fund!moduleid:%u,cmdid:%u,roleId:%lu", roleId, clientMsg.moduleid(), clientMsg.cmdid());
		return false;
	}
	pModule->handleClientMsg(clientMsg.cmdid(), clientMsg.data());
	return true;
}
