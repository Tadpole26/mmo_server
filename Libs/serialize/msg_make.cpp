#include "pch.h"
#include "msg_make.h"
#include "parse_pb.h"
#include "zLogMgr.h"
#include "google/protobuf/message_lite.h"

CMsgMake::CMsgMake() {}
CMsgMake::~CMsgMake() {}

tagMsgHead* CMsgMake::makeHeadMsg(google::protobuf::Message* pMessage, msg_id_t moduleId, uint16 cmdId)
{
	if (pMessage == nullptr) return nullptr;
	size_t nSize = pMessage->ByteSizeLong();
	char* pDataHead = nullptr;

	if (!ResetSend(nSize))
		goto ERROR1;
	pDataHead = (char*)(m_strHead.c_str() + NET_HEAD_SIZE);
	if (nSize > 0 && !SerToString(pMessage, pDataHead, nSize))
	{
		m_strHead.clear();
		m_strHead.reserve(0);
		goto ERROR1;
	}
	return makeHeadMsg(pDataHead, nSize, moduleId, cmdId);

ERROR1:
	Log_Error("%s, sendlen %s : %d, module: %u, protol: %u", typeid(*this).name(), typeid(*pMessage).name(), nSize, moduleId, cmdId);
	return nullptr;
}

tagMsgHead* CMsgMake::makeHeadMsg(const std::string& strMsg, msg_id_t moduleId, uint16 cmdId)
{
	return makeHeadMsg(strMsg.c_str(), strMsg.length(), moduleId, cmdId);
}

tagMsgHead* CMsgMake::makeHeadMsg(const char* buf, size_t size, msg_id_t moduleId, uint16 cmdId)
{
	tagMsgHead* pRet = nullptr;
	if (!ResetSend(size)) goto ERROR1;

	pRet = make_head_msg(buf, size, (char*)m_strHead.c_str(), moduleId, cmdId);
	if (pRet == nullptr)
	{
		m_strHead.clear();
		m_strHead.reserve(0);
		goto ERROR1;
	}
	return pRet;
ERROR1:
	Log_Error("%s, sendlen : %d, moduleId: %u, cmdId: %u", typeid(*this).name(), size, moduleId, cmdId);
	return nullptr;
}

void CMsgMake::ClearMakeCache()
{
	if (m_strHead.size() >= MSG_MAX_LEN)
	{
		m_strHead.clear();
		m_strHead.reserve(0);
	}
}

bool CMsgMake::ResetSend(size_t nSize)
{
	size_t len = NET_HEAD_SIZE + nSize;

	if (m_strHead.size() < len)
		m_strHead.resize(len);

	if (m_strHead.size() < len)
	{
		m_strHead.clear();
		m_strHead.reserve(0);
		return false;
	}
	return true;
}