#include "CGateSession.h"
#include "log_mgr.h"
#include "parse_pb.h"
#include "CUserInfo.h"
#include "CCommonUser.h"
#include "msg_module_serverinner.pb.h"
#include "msg_module_servercommon.pb.h"
#include "CProcessorBase.h"
#include "CModuleProFactory.h"
#include "CUserInfoRecord.h"

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
	if (m_eServerKind == SERVER_KIND_GATE)
		g_UserInfoLRUHashmap->Clear();
	//g_SvrMgr.DelServer(m_eServerKind, m_uiServerId);

	//网关掉发消息
	//if (GAME_LOGIC_INS->IsExit())
	//{
	//	CLoadData::GetInstance()->ServerExitRet(gGameLogic->GetAreanNo(), GetServerKind());
	//	gGameLogic->SetServerState(GetServerKind(), 2);
	//}
}

void CGateSession::handle_msg(const tagMsgHead* pNetMsg)
{
	if (!pNetMsg) return;
	uchar* pBuf = NET_DATA_BUF(pNetMsg);
	uint32 uiLen = NET_DATA_SIZE(pNetMsg);

	switch (pNetMsg->uiCmdId)
	{
	case MsgModule_ServerInner::Msg_ServerInner_GG_Login_Req:
		OnAccountEnter(pBuf, uiLen);
	break;
	case MsgModule_ServerInner::Msg_ServerInner_GG_Create_Req:
		OnCreatePlayer(pBuf, uiLen);
	break;
	case MsgModule_ServerInner::Msg_ServerInner_GG_Save_Ntf:
		OnSavePlayer(pBuf, uiLen);
	break;
	default:
		Log_Error("undefined module %d!", pNetMsg->usModuleId);
	break;
	}
}

void CGateSession::SendLoginErrorRet(int64_t llUid, uint32 uiSeqId, ResultCode eCode)
{
	Msg_ServerInner_GG_Login_Rsp oLoginRsp;
	oLoginRsp.set_lluserid(llUid);
	oLoginRsp.set_iflag(eCode);
	oLoginRsp.set_uiseqid(uiSeqId);
	oLoginRsp.set_brequest(false);
	oLoginRsp.set_estate(ProtoMsg::ePlayer_Offline);
	oLoginRsp.set_llopendate(GetCurrTime());
	this->Send_Msg(&oLoginRsp, MsgModule_ServerInner::Msg_ServerInner_GG_Login_Rsp, MsgModule::ServerInner);
}

void CGateSession::OnAccountEnter(uchar* pMsg, uint32 uiLen)
{
	assert(pMsg);
	Msg_ServerInner_GG_Login_Req oLoginReq;
	PARSE_PTL(oLoginReq, pMsg, uiLen);
	Log_Custom("enter",  "account name=%s, account id=%lld", oLoginReq.straccname().c_str()
		, oLoginReq.llplayerid());

	ResultCode eCode = ResultCode::Code_Common_Success;
	CUserInfo* pUserInfo = CCommonUser::LoadUserInfo(oLoginReq.llplayerid(), eCode);
	if (pUserInfo == nullptr)
	{
		SendLoginErrorRet(oLoginReq.llplayerid(), oLoginReq.uiseqid(), eCode);
		return;
	}
	SendLoginErrorRet(oLoginReq.llplayerid(), oLoginReq.uiseqid(), eCode);
	Log_Info("user account enter, user name=%s, user id=%lld", oLoginReq.straccname().c_str(), oLoginReq.llplayerid());
	return;
}

void CGateSession::OnCreatePlayer(uchar* pMsg, uint32 uiLen)
{
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
}

void CGateSession::OnSavePlayer(uchar* pMsg, uint32 uiLen)
{
	assert(pMsg);
	Msg_ServerInner_GG_Save_Ntf oSaveReq;
	PARSE_PTL(oSaveReq, pMsg, uiLen);
}

void CGateSession::HandleTransmitData(const tagMsgHead* pNetMsg)
{
	Msg_ServerCommon_Transmit oTransmit;
	PARSE_PTL_HEAD(oTransmit, pNetMsg);

	ResultCode eCode = ResultCode::Code_Common_Success;
	CUserInfo* pUserInfo = CCommonUser::GetInCacheUserInfo(oTransmit.lluid());
	if (!pUserInfo)
	{
		Log_Error("user id %lld not find! moduleid:%u, cmdid:%u",oTransmit.lluid(), oTransmit.uimoduleid(), oTransmit.uicmd());
		return;
	}
	pUserInfo->m_iSequence = oTransmit.uisequence();

	//处理普通消息包
	processor_base_ptr_type pProcessor = PROCESSOR_FACTORY_INS.GetProcessor(oTransmit.uimoduleid());
	pProcessor->SetUserBaseInfo(pUserInfo, oTransmit.uicmd());
	pProcessor->DoProcess(oTransmit.strcmdmsg());
	return;
}
