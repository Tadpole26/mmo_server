#pragma once
#include "msg_parser.h"
#include "errorcode.pb.h"

struct tagMsgHead;

class CMsgMake
{
public:
	CMsgMake();
	~CMsgMake();
public:
	tagMsgHead* MakeHeadMsg(google::protobuf::Message* pMessage
		, msg_id_t usModule, msg_id_t usProtocol, uint32_t uiSequence
		, uint32_t eCode = client::ECode_Success);

	tagMsgHead* MakeHeadMsg(const std::string& strMsg
		, msg_id_t usModule, msg_id_t usProtocol, uint32_t uiSequence
		, uint32_t eCode = client::ECode_Success);

	tagMsgHead* MakeHeadMsg(const char* buf, size_t size
		, msg_id_t usModule, msg_id_t usProtocol, uint32_t uiSequence
		, uint32_t eCode = client::ECode_Success);

	void ClearMakeCache();
	bool ResetSend(size_t nSize);
private:
	std::string m_strHead;
};