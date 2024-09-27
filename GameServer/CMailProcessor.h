#pragma once

#include "CProcessorBase.h"
#include "msg_module_mail.pb.h"
#include "parse_pb.h"

class CMailProcessor : public CProcessorBase
{
public:
	CMailProcessor(uint32_t uiModuleId) : CProcessorBase(uiModuleId) { }

	static void UpdateMailNotify(CUserInfo* pUserInfo);

protected:
	bool DoUserRun(const std::string& strMsg) override;

	int GetMailList(const std::string& strMsg, ProtoMsg::Msg_Mail_GetMailList_Rsp& oMailRsp);

	int ReadMail(const std::string& strMsg, ProtoMsg::Msg_Mail_ReadMail_Rsp& oMailRsp);

	int DeleteMail(const std::string& strMsg, ProtoMsg::Msg_Mail_DeleteMail_Rsp& oMailRsp);

	int ReciveAttach(const std::string& strMsg, ProtoMsg::Msg_Mail_ReciveAttach_Rsp& oMailRsp);
};


