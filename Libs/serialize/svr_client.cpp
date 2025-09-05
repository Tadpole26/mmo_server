#include "pch.h"
#include "svr_client.h"
#include "zLogMgr.h"
#include "global_define.h"
#include "google/protobuf/message.h"
#include "google/protobuf/message_lite.h"

svr_session::svr_session() {}
svr_session::~svr_session() {}

bool svr_session::Send_Msg(google::protobuf::Message* pMsg, msg_id_t usProtocol
	, msg_id_t usModule, uint32 code
	, uint32 seqid)
{
	if (pMsg == nullptr)
	{
		Log_Error("module:%u, cmd:%u, class:%s", usModule, usProtocol, typeid(*this).name());
		return false;
	}

	tagMsgHead* pNetMsgHead = MakeHeadMsg(pMsg, usModule, usProtocol, seqid, code);
	if (!pNetMsgHead)
	{
		Log_Error("pNetMsgHead is null!, module:%u, cmd:%u, class:%s"
			, usModule, usProtocol, typeid(*this).name());
		return false;
	}
	bool res = Send(pNetMsgHead);
	CMsgMake::ClearMakeCache();
	return res;
}

bool svr_session::Send_Msg(const std::string& strMsg, msg_id_t usProtocol
	, msg_id_t usModule, uint32 code
	, uint32 seqid)
{
	return Send_Msg((const uchar*)strMsg.c_str(), strMsg.size(), usProtocol, usModule, code, seqid);
}

bool svr_session::Send_Msg(const uchar* buf, size_t size, msg_id_t usProtocol
	, msg_id_t usModule, uint32 code
	, uint32 seqid)
{
	tagMsgHead* pNetMsgHead = MakeHeadMsg((const char*)buf, size, usModule, usProtocol, seqid, code);
	if (!pNetMsgHead)
	{
		Log_Error("pNetMsgHead is null!, module:%u, cmd:%u, class:%s"
			, usModule, usProtocol, typeid(*this).name());
		return false;
	}
	bool res = Send(pNetMsgHead);
	CMsgMake::ClearMakeCache();
	return res;
}

bool svr_session::sendInnerMsg(google::protobuf::Message* pMsg)
{
	if (pMsg == nullptr)
	{
		Log_Error("sendInnerMsg.!pMsg.class:%s", typeid(*this).name());
		return false;
	}
	size_t msgLen = pMsg->ByteSizeLong();
	if (msgLen > MSG_MAX_LEN)
	{
		Log_Error("sendInnerMsg.!msg len too long:%s", pMsg->GetDescriptor()->full_name().c_str());
		return false;
	}
	std::string sendString;
	try
	{
		if (!pMsg->SerializeToString(&sendString))
		{
			Log_Error("ser %s proto, fields:%s!", typeid(*pMsg).name(), pMsg->InitializationErrorString().c_str());
			return false;
		}
		return true;
	}
	catch (const std::string& e)
	{
		Log_Error("ser %s proto, fields:%s!, error:%s!", typeid(*pMsg).name(), pMsg->InitializationErrorString().c_str(), e.c_str());
	}
	char* pMsgData = (char*)(sendString.c_str());
	bool res = __sendInnerMsg(pMsgData, msgLen);
	return res;
}

void svr_session::Init()
{
	m_hd.m_threadOid = invalid_thread_oid;
	m_hd.m_connOid = invalid_conn_oid;

	SetServerId(0);
	SetServerKind(SERVER_KIND_NONE);
}

void svr_session::Release()
{
	m_hd.m_threadOid = invalid_thread_oid;
	m_hd.m_connOid = invalid_conn_oid;

	SetServerId(0);
	SetServerKind(SERVER_KIND_NONE);
}

bool svr_session::Send(const tagMsgHead* pMsg)
{
	if (pMsg == nullptr) return false;

	bool bRes = CTcpSession::_Send(pMsg);
	if (!bRes)
	{
		Log_Error("pNetMsgHead is null!, module:%u, cmd:%u", pMsg->usModuleId, pMsg->uiCmdId);
		return false;
	}
	return bRes;
}

void svr_session::handle_msgv(const void* pMsg)
{
	handle_msg(NET_HEAD_MSG(pMsg));
}

////////////////////////////////////////////////////////////////////////////
svr_reconn::svr_reconn(bool bCache, uint32 CacheSec)
	: CTcpReconn(bCache, CacheSec)
{
}

svr_reconn::~svr_reconn()
{
}

bool svr_reconn::Send_Msg(google::protobuf::Message* pMsg, msg_id_t usProtocol
	, msg_id_t usModule, uint32 code
	, uint32 seqid)
{
	if (pMsg == nullptr)
	{
		Log_Error("Moduleid:%u, cmd:%u, class:%s", usModule, usProtocol, typeid(*this).name());
		return false;
	}
	tagMsgHead* pNetMsgHead = MakeHeadMsg(pMsg, usModule, usProtocol, seqid, code);
	if (!pNetMsgHead)
	{
		Log_Error("pNetMsgHead is null!, module:%u, cmd:%u, class:%s"
			, usModule, usProtocol, typeid(*this).name());
		return false;
	}
	bool res = Send(pNetMsgHead);
	CMsgMake::ClearMakeCache();
	return res;
}

bool svr_reconn::Send_Msg(const std::string& strMsg, msg_id_t usProtocol
	, msg_id_t usModule, uint32 code
	, uint32 seqid)
{
	return Send_Msg((const uchar*)strMsg.c_str()
		, strMsg.size(), usProtocol, usModule, code, seqid);
}

bool svr_reconn::Send_Msg(const uchar* buf, size_t size, msg_id_t usProtocol
	, msg_id_t usModule, uint32 code
	, uint32 seqid)
{
	tagMsgHead* pNetMsgHead = MakeHeadMsg((const char*)buf, size, usModule, usProtocol, seqid, code);
	if (!pNetMsgHead)
	{
		Log_Error("pNetMsgHead is null!, module:%u, cmd:%u, class:%s"
			, usModule, usProtocol, typeid(*this).name());
		return false;
	}
	bool res = Send(pNetMsgHead);
	CMsgMake::ClearMakeCache();
	return res;
}

bool svr_reconn::Send(const tagMsgHead* pMsg)
{
	if (pMsg == nullptr) return false;
	bool bRes = CTcpReconn::_Send(pMsg);
	if (!bRes && is_connected())
	{
		Log_Error("pNetMsgHead is null!, module:%u, cmd:%u", pMsg->usModuleId, pMsg->uiCmdId);
		return false;
	}
	return bRes;
}

bool svr_reconn::sendInnerMsg(google::protobuf::Message* pMsg)
{
	if (pMsg == nullptr)
	{
		Log_Error("sendInnerMsg.!pMsg.class:%s", typeid(*this).name());
		return false;
	}
	size_t msgLen = pMsg->ByteSizeLong();
	if (msgLen > MSG_MAX_LEN)
	{
		Log_Error("sendInnerMsg.!msg len too long:%s", pMsg->GetDescriptor()->full_name().c_str());
		return false;
	}
	std::string sendString;
	try
	{
		if (!pMsg->SerializeToString(&sendString))
		{
			Log_Error("ser %s proto, fields:%s!", typeid(*pMsg).name(), pMsg->InitializationErrorString().c_str());
			return false;
		}
		return true;
	}
	catch (const std::string& e)
	{
		Log_Error("ser %s proto, fields:%s!, error:%s!", typeid(*pMsg).name(), pMsg->InitializationErrorString().c_str(), e.c_str());
	}
	char* pMsgData = (char*)(sendString.c_str());
	bool res = __sendInnerMsg(pMsgData, msgLen);
	return res;
}

void svr_reconn::handle_msgv(const void* pMsg)
{
	handle_msg(NET_HEAD_MSG(pMsg));
}