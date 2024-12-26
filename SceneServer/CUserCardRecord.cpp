#include "CUserCardRecord.h"
#include <cassert>

const char* CUserCardRecord::s_strFirstName = "u_user";
const char* CUserCardRecord::s_strSecName = "user_card_list";
const char* CUserCardRecord::s_strSecType = "list";
const char* CUserCardRecord::s_strId = "id";
const char* CUserCardRecord::s_strCardLevel = "card_level";
const char* CUserCardRecord::s_strCardColor = "card_color";
const char* CUserCardRecord::s_strCardStar = "card_star";

CUserCardRecord::~CUserCardRecord()
{
    if (m_pCache != NULL)
        m_pCache->m_pRecord = NULL;
}

int64_t CUserCardRecord::GetUid()
{
    return m_llUid;
}
int CUserCardRecord::GetId()
{
    return m_iId;
}

shared_ptr<sDBRequest> CUserCardRecord::MakeInsertSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_IncUpdate, 
            CUserCardRecord::s_strFirstName, 
            CUserCardRecord::s_strSecName);
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

shared_ptr<sDBRequest> CUserCardRecord::MakeUpdateSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_Update, 
            CUserCardRecord::s_strFirstName, 
            CUserCardRecord::s_strSecName);
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
 
shared_ptr<sDBRequest> CUserCardRecord::MakeDeleteSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_Delete, 
            CUserCardRecord::s_strFirstName, 
            CUserCardRecord::s_strSecName);
    pRequest->AddCon("_id", GetUid());
    pRequest->AddCon("id", GetId());
    return pRequest;
}


std::vector<user_card_table_value_type> CUserCardRecord::SelectDataFromDB(int64_t llUid, sDBSecRet& vecData)
{
    std::vector<user_card_table_value_type> vecRet;
    for (sDBMap& oValue : vecData)
    {
        user_card_table_value_type stData;
        stData.m_llUid = llUid;
        stData.m_iId = oValue.NumInt("id");
        stData.m_iCardLevel = oValue.NumInt("card_level");
        stData.m_iCardColor = oValue.NumInt("card_color");
        stData.m_iCardStar = oValue.NumInt("card_star");
        vecRet.push_back(stData);
    }
    return vecRet;
}