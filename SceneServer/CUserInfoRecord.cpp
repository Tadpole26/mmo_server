#include "CUserInfoRecord.h"
#include <cassert>

const char* CUserInfoRecord::s_strFirstName = "u_user";
const char* CUserInfoRecord::s_strSecName = "user_info_object";
const char* CUserInfoRecord::s_strSecType = "object";
const char* CUserInfoRecord::s_strUserName = "user_name";
const char* CUserInfoRecord::s_strUserLevel = "user_level";
const char* CUserInfoRecord::s_strUserExp = "user_exp";
const char* CUserInfoRecord::s_strCreateTime = "create_time";
const char* CUserInfoRecord::s_strVipLevel = "vip_level";
const char* CUserInfoRecord::s_strVipExp = "vip_exp";
const char* CUserInfoRecord::s_strInnerMailAddId = "inner_mail_add_id";

CUserInfoRecord::~CUserInfoRecord()
{
    if (m_pCache != NULL)
        m_pCache->m_pRecord = NULL;
}

int64_t CUserInfoRecord::GetUid()
{
    return m_llUid;
}

shared_ptr<sDBRequest> CUserInfoRecord::MakeInsertSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_IncUpdate, 
            CUserInfoRecord::s_strFirstName, 
            CUserInfoRecord::s_strSecName, 
            tagSecTblDesc::eST_Object);
    pRequest->AddCon("_id", GetUid());
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

shared_ptr<sDBRequest> CUserInfoRecord::MakeUpdateSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_Update, 
            CUserInfoRecord::s_strFirstName, 
            CUserInfoRecord::s_strSecName, 
            tagSecTblDesc::eST_Object);
    pRequest->AddCon("_id", GetUid());
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
 
shared_ptr<sDBRequest> CUserInfoRecord::MakeDeleteSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_DropParam, 
            CUserInfoRecord::s_strFirstName);
    pRequest->AddField(CUserInfoRecord::s_strSecName);
    return pRequest;
}


std::vector<user_info_table_value_type> CUserInfoRecord::SelectDataFromDB(int64_t llUid, sDBSecRet& vecData)
{
    std::vector<user_info_table_value_type> vecRet;
    for (sDBMap& oValue : vecData)
    {
        user_info_table_value_type stData;
        stData.m_llUid = llUid;
        stData.m_strUserName = oValue.Str("user_name");
        stData.m_iUserLevel = oValue.NumInt("user_level");
        stData.m_iUserExp = oValue.NumInt("user_exp");
        stData.m_llCreateTime = oValue.Time("create_time");
        stData.m_iVipLevel = oValue.NumInt("vip_level");
        stData.m_iVipExp = oValue.NumInt("vip_exp");
        stData.m_llInnerMailAddId = oValue.NumLong("inner_mail_add_id");
        vecRet.push_back(stData);
    }
    return vecRet;
}