#include "parse_pb.h"
#include "log_mgr.h"
#include "CFriendNetface.h"

CFriendNetface::CFriendNetface()
{
}

CFriendNetface::~CFriendNetface()
{
}

void CFriendNetface::on_connect()
{
	/*
	Msg_ServerCommon_Register_Req oRegisterReq;
	oRegisterReq.set_uiserverid(GetServerID());
	oRegisterReq.set_uiserverkind(SERVER_KIND_GAME);
	//oRegisterReq.set_strexip(GAME_LOGIC_INS->);
	oRegisterReq.set_uiindex(GAME_LOGIC_INS->m_stArgOpt.GetIndex());
	oRegisterReq.set_uiplatid(GAME_LOGIC_INS->m_oConstCfg.m_uiPlatId);
	oRegisterReq.set_uigroupid(GAME_LOGIC_INS->m_oConstCfg.m_uiGroupId);
	//oRegisterReq.set_open
	Send_Msg(&oRegisterReq, MsgModule_ServerCommon::Msg_ServerCommon_Register_Req, MsgModule::ServerCommon);
	CTcpReconn::on_connect();
	*/
	Log_Warning("friend server connect!");
}

void CFriendNetface::on_disconnect()
{
	Log_Warning("friend server disconnect!");
}

void CFriendNetface::trigger()
{
}

bool CFriendNetface::NetMsgFromFriend(const tagMsgHead* pMsgHead)
{
	assert(pMsgHead);
	return true;
}