#include "CUserItemRecord.h"
#include <cassert>

const char* CUserItemRecord::s_strFirstName = "u_user";
const char* CUserItemRecord::s_strSecName = "user_item_list";
const char* CUserItemRecord::s_strSecType = "list";
const char* CUserItemRecord::s_strId = "id";
const char* CUserItemRecord::s_strItemNum = "item_num";
const char* CUserItemRecord::s_strItemTime = "item_time";

CUserItemRecord::~CUserItemRecord()
{
    if (m_pCache != NULL)
        m_pCache->m_pRecord = NULL;
}

int64_t CUserItemRecord::GetUid()
{
    return m_llUid;
}
int CUserItemRecord::GetId()
{
    return m_iId;
}

shared_ptr<sDBRequest> CUserItemRecord::MakeInsertSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_IncUpdate, 
            CUserItemRecord::s_strFirstName, 
            CUserItemRecord::s_strSecName);
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

shared_ptr<sDBRequest> CUserItemRecord::MakeUpdateSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_Update, 
            CUserItemRecord::s_strFirstName, 
            CUserItemRecord::s_strSecName);
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
 
shared_ptr<sDBRequest> CUserItemRecord::MakeDeleteSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_Delete, 
            CUserItemRecord::s_strFirstName, 
            CUserItemRecord::s_strSecName);
    pRequest->AddCon("_id", GetUid());
    pRequest->AddCon("id", GetId());
    return pRequest;
}


std::vector<user_item_table_value_type> CUserItemRecord::SelectDataFromDB(int64_t llUid, sDBSecRet& vecData)
{
    std::vector<user_item_table_value_type> vecRet;
    for (sDBMap& oValue : vecData)
    {
        user_item_table_value_type stData;
        stData.m_llUid = llUid;
        stData.m_iId = oValue.NumInt("id");
        stData.m_iItemNum = oValue.NumInt("item_num");
        stData.m_llItemTime = oValue.Time("item_time");
        vecRet.push_back(stData);
    }
    return vecRet;
}