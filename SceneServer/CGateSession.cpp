#include "scenesvr.pb.h"

#include "CGateSession.h"
#include "GameModule.h"
#include "GameUser.h"
#include "GameUserMgr.h"
#include "parse_pb.h"

CGateSession::CGateSession()
{
	SetServerKind(SERVER_KIND_GATE);
	SetServerId(0);
}

CGateSession::~CGateSession()
{
}

void CGateSession::on_disconnect()
{
	Log_Warning("GateSession %u disconnect!", m_uiServerId);
}

void CGateSession::handle_msg(const tagMsgHead* pNetMsg)
{
	Log_Info("handle_msg");
	return;
}

void CGateSession::SendLoginErrorRet(int64_t llUid, uint32 uiSeqId, uint32 eCode)
{
	/*
	Msg_ServerInner_GG_Login_Rsp oLoginRsp;
	oLoginRsp.set_lluserid(llUid);
	oLoginRsp.set_iflag(eCode);
	oLoginRsp.set_uiseqid(uiSeqId);
	oLoginRsp.set_brequest(false);
	oLoginRsp.set_estate(ProtoMsg::ePlayer_Offline);
	oLoginRsp.set_llopendate(GetCurrTime());
	this->Send_Msg(&oLoginRsp, MsgModule_ServerInner::Msg_ServerInner_GG_Login_Rsp, client::enModule_ServerInner);
	*/
}


void CGateSession::OnCreateRole(const inner::InnerScenesvr& innerReq)
{
	zRoleIdType roleId = innerReq.fromuser();
	auto& createRole = innerReq.fromgate_createrole();
	auto *pUser = gGameUserMgrIns->getUser(roleId);
	if (pUser)
	{
		Log_Error("OnCreateRole.pUser.%lu", roleId);
		return;
	}
	pUser = gGameUserMgrIns->createUser(roleId);
	if (!pUser)
	{
		Log_Error("OnCreateRole.!pUser.%lu", roleId);
		return;
	}
	Log_Info("create role, roleId%lu", roleId);

}

bool CGateSession::handClientMsg(const inner::InnerScenesvr& innerReq)
{
	zRoleIdType roleId = innerReq.fromuser();
	auto &clientMsg = innerReq.fromgate_clientmsg();
	auto *pUser = gGameUserMgrIns->getUser(roleId);
	if (!pUser)
	{
		Log_Error("user id %lu not find!moduleid:%u,cmdid:%u",roleId, clientMsg.moduleid(), clientMsg.cmdid());
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
