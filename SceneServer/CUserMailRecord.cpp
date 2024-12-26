#include "CUserMailRecord.h"
#include <cassert>

const char* CUserMailRecord::s_strFirstName = "u_user";
const char* CUserMailRecord::s_strSecName = "user_mail_list";
const char* CUserMailRecord::s_strSecType = "list";
const char* CUserMailRecord::s_strId = "id";
const char* CUserMailRecord::s_strMailType = "mail_type";
const char* CUserMailRecord::s_strSendTime = "send_time";
const char* CUserMailRecord::s_strExpiredTime = "expired_time";
const char* CUserMailRecord::s_strIsDelete = "is_delete";
const char* CUserMailRecord::s_strIsReceived = "is_received";
const char* CUserMailRecord::s_strTitile = "titile";
const char* CUserMailRecord::s_strSender = "sender";
const char* CUserMailRecord::s_strContent = "content";
const char* CUserMailRecord::s_strAttachment = "attachment";

CUserMailRecord::~CUserMailRecord()
{
    if (m_pCache != NULL)
        m_pCache->m_pRecord = NULL;
}

int64_t CUserMailRecord::GetUid()
{
    return m_llUid;
}
int64_t CUserMailRecord::GetId()
{
    return m_llId;
}

shared_ptr<sDBRequest> CUserMailRecord::MakeInsertSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_IncUpdate, 
            CUserMailRecord::s_strFirstName, 
            CUserMailRecord::s_strSecName);
    pRequest->AddCon("_id", GetUid());
    pRequest->AddCon("id", GetId());
    for (const auto& content : m_mapInt)
    {
        pRequest->AddReq(content.first, content.second);
    }
    for (const auto& content : m_mapUint32)
    {
        pRequest->AddReq(content.first, content.second);
    }
    for (const auto& content : m_mapInt64)
    {
        pRequest->AddReq(content.first, content.second);
    }
    for (const auto& content : m_mapTime)
    {
        pRequest->AddReqTime(content.first, content.second);
    }
    for (const auto& content : m_mapString)
    {
        pRequest->AddReqStr(content.first, content.second);
    }
    for (const auto& content : m_mapBool)
    {
        pRequest->AddBool(content.first, content.second);
    }

    return pRequest;
}

shared_ptr<sDBRequest> CUserMailRecord::MakeUpdateSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_Update, 
            CUserMailRecord::s_strFirstName, 
            CUserMailRecord::s_strSecName);
    pRequest->AddCon("_id", GetUid());
    pRequest->AddCon("id", GetId());
    for (const auto& content : m_mapInt)
    {
        pRequest->AddReq(content.first, content.second);
    }
    for (const auto& content : m_mapUint32)
    {
        pRequest->AddReq(content.first, content.second);
    }
    for (const auto& content : m_mapInt64)
    {
        pRequest->AddReq(content.first, content.second);
    }
    for (const auto& content : m_mapTime)
    {
        pRequest->AddReqTime(content.first, content.second);
    }
    for (const auto& content : m_mapString)
    {
        pRequest->AddReqStr(content.first, content.second);
    }
    for (const auto& content : m_mapBool)
    {
        pRequest->AddBool(content.first, content.second);
    }

    return pRequest;
}
 
shared_ptr<sDBRequest> CUserMailRecord::MakeDeleteSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_Delete, 
            CUserMailRecord::s_strFirstName, 
            CUserMailRecord::s_strSecName);
    pRequest->AddCon("_id", GetUid());
    pRequest->AddCon("id", GetId());
    return pRequest;
}


std::vector<user_mail_table_value_type> CUserMailRecord::SelectDataFromDB(int64_t llUid, sDBSecRet& vecData)
{
    std::vector<user_mail_table_value_type> vecRet;
    for (sDBMap& oValue : vecData)
    {
        user_mail_table_value_type stData;
        stData.m_llUid = llUid;
        stData.m_llId = oValue.NumLong("id");
        stData.m_iMailType = oValue.NumInt("mail_type");
        stData.m_llSendTime = oValue.Time("send_time");
        stData.m_llExpiredTime = oValue.Time("expired_time");
        stData.m_bIsDelete = oValue.Bool("is_delete");
        stData.m_bIsReceived = oValue.Bool("is_received");
        stData.m_strTitile = oValue.Str("titile");
        stData.m_strSender = oValue.Str("sender");
        stData.m_strContent = oValue.Str("content");
        stData.m_strAttachment = oValue.Str("attachment");
        vecRet.push_back(stData);
    }
    return vecRet;
}