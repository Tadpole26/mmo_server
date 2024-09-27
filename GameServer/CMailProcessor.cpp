#include "CMailProcessor.h"
#include "CModuleProFactory.h"
#include "msg_module.pb.h"
#include "msg_module_mail.pb.h"
#include "result_code.pb.h"
#include "CUserInfo.h"
#include "CItemConfig.h"
#include "CCommonItem.h"
#include "CGlobalMailMgr.h"

REGISTER_MODULE_FACTORY(MsgModule::Mail, CMailProcessor)

bool
CMailProcessor::DoUserRun(const std::string& strMsg)
{
    if (m_uiCmd == MsgModule_Mail::Msg_Mail_GetMailList_Req)
    {
        ProtoMsg::Msg_Mail_GetMailList_Rsp  oMailRsp;
        ResultCode eCode = (ResultCode)GetMailList(strMsg, oMailRsp);
        SendToClient(MsgModule::Mail, MsgModule_Mail::Msg_Mail_GetMailList_Rsp, &oMailRsp, eCode);
    }
    else if (m_uiCmd == MsgModule_Mail::Msg_Mail_ReadMail_Req)
    {
        ProtoMsg::Msg_Mail_ReadMail_Rsp oMailRsp;
        ResultCode eCode = (ResultCode)ReadMail(strMsg, oMailRsp);
        SendToClient(MsgModule::Mail, MsgModule_Mail::Msg_Mail_ReadMail_Rsp, &oMailRsp, eCode);
    }
    else if (m_uiCmd == MsgModule_Mail::Msg_Mail_ReadMail_Req)
    {
        ProtoMsg::Msg_Mail_ReadMail_Rsp oMailRsp;
        ResultCode eCode = (ResultCode)ReadMail(strMsg, oMailRsp);
        SendToClient(MsgModule::Mail, MsgModule_Mail::Msg_Mail_ReadMail_Rsp, &oMailRsp, eCode);
    }
    else if (m_uiCmd == MsgModule_Mail::Msg_Mail_DeleteMail_Req)
    {
        ProtoMsg::Msg_Mail_DeleteMail_Rsp oMailRsp;
        ResultCode eCode = (ResultCode)DeleteMail(strMsg, oMailRsp);
        SendToClient(MsgModule::Mail, MsgModule_Mail::Msg_Mail_DeleteMail_Rsp, &oMailRsp, eCode);
    }
    else if (m_uiCmd == MsgModule_Mail::Msg_Mail_ReciveAttach_Req)
    {
        ProtoMsg::Msg_Mail_ReciveAttach_Rsp oMailRsp;
        ResultCode eCode = (ResultCode)ReciveAttach(strMsg, oMailRsp);
        SendToClient(MsgModule::Mail, MsgModule_Mail::Msg_Mail_ReciveAttach_Rsp, &oMailRsp, eCode);
    }
    else
    {
        Log_Error("unknown cmd:%u", m_uiCmd);
    }
    return (true);
}

void CMailProcessor::UpdateMailNotify(CUserInfo* pUserInfo)
{
    ProtoMsg::Msg_Mail_MailList_Notify oNotify;
    for (auto iter = pUserInfo->m_mapUserMail.Begin(); iter != pUserInfo->m_mapUserMail.End(); ++iter)
    {
        if (iter->second->GetIsDelete())
            continue;

        ProtoMsg::mail_info_t* pMsgMail = oNotify.add_vecmail();
        if (pMsgMail == nullptr) continue;
        pMsgMail->set_llmailid(iter->second->GetId());
        pMsgMail->set_etype((ProtoMsg::enum_mail_t)iter->second->GetMailType());
        pMsgMail->set_llbegintime(iter->second->GetSendTime());
        pMsgMail->set_llexpiredtime(iter->second->GetExpiredTime());
        pMsgMail->set_brecived(iter->second->GetIsReceived());
        pMsgMail->set_strtitle(iter->second->GetTitile());
        pMsgMail->set_strsender(iter->second->GetSender());;
        pMsgMail->set_strcontent(iter->second->GetContent());
    }
    pUserInfo->SendToClient(MsgModule::Mail, MsgModule_Mail::Msg_Mail_MailList_Notify, &oNotify, Code_Common_Success, true);

}

int CMailProcessor::GetMailList(const std::string& strMsg, ProtoMsg::Msg_Mail_GetMailList_Rsp& oMailRsp)
{
    //检测系统新邮件
    GLOBAL_MAIL_INS->CheckGlobalMail(m_pUserInfo);

    for (auto iter = m_pUserInfo->m_mapUserMail.Begin(); iter != m_pUserInfo->m_mapUserMail.End(); ++iter)
    {
        if (iter->second->GetIsDelete())
            continue;

        ProtoMsg::mail_info_t* pMsgMail = oMailRsp.add_vecmail();
        if (pMsgMail == nullptr) continue;
        pMsgMail->set_llmailid(iter->second->GetId());
        pMsgMail->set_etype((ProtoMsg::enum_mail_t)iter->second->GetMailType());
        pMsgMail->set_llbegintime(iter->second->GetSendTime());
        pMsgMail->set_llexpiredtime(iter->second->GetExpiredTime());
        pMsgMail->set_brecived(iter->second->GetIsReceived());
        pMsgMail->set_strtitle(iter->second->GetTitile());
        pMsgMail->set_strsender(iter->second->GetSender());;
        pMsgMail->set_strcontent(iter->second->GetContent());
    }
    return Code_Common_Success;
}

int CMailProcessor::ReadMail(const std::string& strMsg, ProtoMsg::Msg_Mail_ReadMail_Rsp& oMailRsp)
{
    ProtoMsg::Msg_Mail_ReadMail_Req oMailReq;
    PARSE_PTL_STR_RET_FAIL(oMailReq, strMsg);

    auto iterMail = m_pUserInfo->m_mapUserMail.Find(oMailReq.llmailid());
    if (iterMail == m_pUserInfo->m_mapUserMail.End())
    {
        Log_Error("read mail is not exist, %lld", oMailReq.llmailid());
        return Code_Common_Failure;
    }
    return Code_Common_Success;
}

int CMailProcessor::DeleteMail(const std::string& strMsg, ProtoMsg::Msg_Mail_DeleteMail_Rsp& oMailRsp)
{
    ProtoMsg::Msg_Mail_DeleteMail_Req oMailReq;
    PARSE_PTL_STR_RET_FAIL(oMailReq, strMsg);

    auto iterMail = m_pUserInfo->m_mapUserMail.Find(oMailReq.llmailid());
    if (iterMail == m_pUserInfo->m_mapUserMail.End())
    {
        Log_Error("delete mail is not exist, %lld", oMailReq.llmailid());
        return Code_Common_Failure;
    }

    if (iterMail->second->GetIsDelete())
    {
        Log_Error("mail already deleted, %lld", oMailReq.llmailid());
        return Code_Common_Failure;
    }

    iterMail->second->SetIsDelete(true);

    oMailRsp.set_llmailid(oMailReq.llmailid());
    return Code_Common_Success;
}

int CMailProcessor::ReciveAttach(const std::string& strMsg, ProtoMsg::Msg_Mail_ReciveAttach_Rsp& oMailRsp)
{
    ProtoMsg::Msg_Mail_ReciveAttach_Req oMailReq;
    PARSE_PTL_STR_RET_FAIL(oMailReq, strMsg);

    auto iterMail = m_pUserInfo->m_mapUserMail.Find(oMailReq.llmailid());
    if (iterMail == m_pUserInfo->m_mapUserMail.End())
    {
        Log_Error("recive mail attachment is not exist, %lld", oMailReq.llmailid());
        return Code_Common_Failure;
    }

    if (iterMail->second->GetIsReceived())
    {
        Log_Error("mail attachment already recive, %lld", oMailReq.llmailid());
        return Code_Common_Failure;
    }
    
    std::vector<game_item_t> vecAward, vecOutAward;
    if (!CCommonItem::ParseItem(iterMail->second->GetAttachment(), vecAward) || vecAward.empty())
    {
        Log_Error("mail attachment award is empty, %lld", oMailReq.llmailid());
        return Code_Common_Failure;
    }

    CCommonItem::AwardItem(m_pUserInfo, vecAward, vecOutAward);
    iterMail->second->SetIsReceived(true);

    oMailRsp.set_llmailid(oMailReq.llmailid());
    for (auto& outItem : vecOutAward)
    {
        game_item_t* pMsgItem = oMailRsp.add_vecitem();
        if (pMsgItem == nullptr) continue;
        pMsgItem->CopyFrom(outItem);
    }
    return Code_Common_Success;
}
