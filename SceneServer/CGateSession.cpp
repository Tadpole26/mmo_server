#include "scenesvr.pb.h"

#include "CGateSession.h"
#include "GameUserMgr.h"
#include "parse_pb.h"

CGateSession::CGateSession()
{
	SetServerKind(SERVER_KIND_NONE);
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
	if (!pNetMsg) return;
	uchar* pBuf = NET_DATA_BUF(pNetMsg);
	uint32 uiLen = NET_DATA_SIZE(pNetMsg);
	Inner::InnerScenesvr_Fromgateway_ClientMsg innerReq;
	PARSE_PTL(innerReq, pBuf, uiLen);
	switch (innerReq.Fromgate_case())
	{
	case inner::kFromgatewayClientmsg:
		break;
	default:
		Log_Error("undefined module %u!", innerReq.Fromgate_case());
	break;
	}
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


void CGateSession::OnCreateRole(uchar* pMsg, uint32 uiLen)
{
	/*
	assert(pMsg);
	Msg_ServerInner_GG_Create_Req oCreateReq;
	PARSE_PTL(oCreateReq, pMsg, uiLen);

	ResultCode eCode = ResultCode::Code_Common_Success;
	CUserInfo* pUserInfo = CCommonUser::LoadUserInfo(oCreateReq.llplayerid(), eCode);
	if (pUserInfo != nullptr)
	{
		Log_Error("user is already exist in game server, user id:%lld", oCreateReq.llplayerid());
		return;
	}

	Log_Custom("create", "create user, user id:%lld", oCreateReq.llplayerid());

	user_info_table_t stUserBaseInfo;
	user_info_table_value_type stValue;
	stValue.m_llUid = oCreateReq.llplayerid();
	stValue.m_llCreateTime = GetCurrTime();
	stValue.m_iUserLevel = 9999;
	stValue.m_iVipLevel = 5;
	stValue.m_iVipExp = 300;
	stUserBaseInfo.Initialize(stValue);
	SQL_TABLE_RECORD_CACHE_INS->ExecSQL();

	//CNetProxy::OnGetUniqueIdReq(oCreateReq.llplayerid(), oCreateReq.strplayername(), CGameLogic::GetInstance()->GetAreanNo());
	*/
}

void CGateSession::handClientMsg(const InnerGatesvr& innerReq)
{
	zRoleIdType roleId = innerReq.fromuser();
	auto &clientMsg = innerReq.fromgate_clientmsg();
	auto *pUser = gGameUserMgrIns->getUser(roleId);
	if (!pUser)
	{
		Log_Error("user id %lu not find! moduleid:%u, cmdid:%u",roleId, clientMsg.moduleid(), clientMsg.cmdid());
		return;
	}
	auto *pModule = pUser->
	//������ͨ��Ϣ��
	return;
}
