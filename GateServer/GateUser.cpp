#include "client.pb.h"
#include "login.pb.h"
#include "scenesvr.pb.h"

#include "GateUser.h"
#include "util_time.h"
#include "zLogMgr.h"
#include "CGateLogic.h"
#include "CGameNetface.h"
#include "PlayerProxy.h"
#include "msg_parser.h"

GateUser::GateUser(zRoleIdType roleId) : cUser(roleId)
{
}

GateUser::~GateUser() {}

bool GateUser::init()
{
	return true;
}

void GateUser::final()
{
	m_hd.m_threadOid = invalid_thread_oid;
	m_hd.m_connOid = invalid_conn_oid;
	cUser::final();
}

void GateUser::sendCmdToSceneSvr(const tagMsgHead* pNetMsgHead)
{
	inner::InnerScenesvr ntf;
	ntf.set_fromuser(getRoleId());
	auto& clientMsg = *ntf.mutable_fromgateway_clientmsg();
	clientMsg.set_module(pNetMsgHead->usModuleId);
	clientMsg.set_cmd(pNetMsgHead->uiCmdId);
	clientMsg.set_data(NET_DATA_BUF(pNetMsgHead), NET_DATA_SIZE(pNetMsgHead));
	gGateLogicIns->m_pGameNetface->Send_Msg(&ntf,26);
}

void GateUser::handle_msg(const tagMsgHead* pNetMsg)
{
	//客户端和服务器之间的心跳包,在网络层收到消息更新时间,网络层长时间没有收到包会将socket断开
	if (pNetMsg->usModuleId == client::enModule_Login && pNetMsg->uiCmdId == client::enModuleLogin_Req_KeepHeart)
	{
		static client::ModuleLogin_Ntf_KeepHeart ntf;
		ntf.set_servertime(GetCurrTime());
		sendCmdToMe(&ntf, client::enModule_Login,client::enModuleLogin_Ntf_KeepHeart, pNetMsg->uiSeqid, 0);
		return;
	}
	sendCmdToSceneSvr(pNetMsg);
	return;
}

void GateUser::on_disconnect()
{
	/*
	if (m_uiGameId != 0)
	{
		Msg_ServerInner_GG_Save_Ntf oSavePlayer;
		oSavePlayer.set_llplayerid(m_llUid);
		oSavePlayer.set_uisaveflag(m_bSaveFlag);
		oSavePlayer.set_strmsg("");
		oSavePlayer.set_uigateid(GATE_LOGIC_INS->m_oConstConfig.m_uiGroupId);
		GATE_LOGIC_INS->m_pGameNetface->Send_Msg(&oSavePlayer,
			MsgModule_ServerInner::Msg_ServerInner_GG_Save_Ntf, MsgModule::ServerInner);
		if (m_stStat.m_eStatus != eUserStatus::STATUS_ReKicking)
			CPlayerProxy::NotifyPlayerOffline(this);
	}*/
	Log_Custom("enter", "disconnect roleid:%lu, threadid:%u, conid:%u timeout:%d", getRoleId(), m_hd.m_threadOid, m_hd.m_connOid, IsTimeOut());
	//GATE_LOGIC_INS->m_oUserMgr.DelGamePlayer(m_llUid);
}

bool GateUser::Send(const tagMsgHead* pMsg)
{
	return svr_session::Send(pMsg);
}

void GateUser::sendCmdToMe(google::protobuf::Message* pMessage, uint32 module, msg_id_t cmd, uint32 uiSequence, uint32 eCode)
{
	//Send_Msg(pMessage, cmd, module, eCode, uiSequence);
}


void GateUser::sendCmdToMe(const std::string& strMsg, uint32 module, msg_id_t cmd, uint32 uiSequence, uint32 eCode)
{
	//Send_Msg(strMsg, usProtocol, usModule, eCode, uiSequence);
}

bool GateUser::UpdateStatus(eUserStatus eStatus)
{
	m_stStat.m_eStatus = eStatus;
	m_stStat.m_llRecTime = GetCurrTime();
	return true;
}

