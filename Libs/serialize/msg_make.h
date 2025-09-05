#pragma once
#include "msg_parser.h"
#include "sdk.h"
#include "google/protobuf/message.h"
struct tagMsgHead;

class CMsgMake
{
public:
	CMsgMake();
	~CMsgMake();
public:
	tagMsgHead* makeHeadMsg(google::protobuf::Message* pMessage, msg_id_t moduleId, uint16 cmdId);
	tagMsgHead* makeHeadMsg(const std::string& strMsg, msg_id_t moduleId, uint16 cmdId);
	tagMsgHead* makeHeadMsg(const char* buf, size_t size, msg_id_t moduleId, uint16 cmdId);

	void ClearMakeCache();
	bool ResetSend(size_t nSize);
private:
	std::string m_strHead;
};