#include "CGlobalRankerRecord.h"
#include <cassert>

const char* CGlobalRankerRecord::s_strFirstName = "u_ranker";
const char* CGlobalRankerRecord::s_strSecType = "list";
const char* CGlobalRankerRecord::s_strRankerList = "ranker_list";
const char* CGlobalRankerRecord::s_strResetTime = "reset_time";

CGlobalRankerRecord::~CGlobalRankerRecord()
{
    if (m_pCache != NULL)
        m_pCache->m_pRecord = NULL;
}

int64_t CGlobalRankerRecord::GetUid()
{
    return m_llUid;
}

shared_ptr<sDBRequest> CGlobalRankerRecord::MakeInsertSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_IncUpdate, 
            CGlobalRankerRecord::s_strFirstName);
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

shared_ptr<sDBRequest> CGlobalRankerRecord::MakeUpdateSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_Update, 
            CGlobalRankerRecord::s_strFirstName);
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
 
shared_ptr<sDBRequest> CGlobalRankerRecord::MakeDeleteSQL()
{
    std::shared_ptr<sDBRequest> pRequest = std::make_shared<sDBRequest>(eDB_Delete, 
            CGlobalRankerRecord::s_strFirstName);
    pRequest->AddCon("_id", GetUid());
    return pRequest;
}


std::vector<global_ranker_table_value_type> CGlobalRankerRecord::SelectDataFromDB(int64_t llUid, sDBSecRet& vecData)
{
    std::vector<global_ranker_table_value_type> vecRet;
    for (sDBMap& oValue : vecData)
    {
        global_ranker_table_value_type stData;
        stData.m_llUid = llUid;
        stData.m_strRankerList = oValue.Str("ranker_list");
        stData.m_llResetTime = oValue.Time("reset_time");
        vecRet.push_back(stData);
    }
    return vecRet;
}