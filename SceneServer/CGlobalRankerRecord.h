#pragma once
#include "CTableRecordBase.h"
#include "CTableRecordCache.h"

struct global_ranker_table_t;
struct global_ranker_table_value_type;
class CGlobalRankerRecord : public CTableRecordBase
{
public:
    CGlobalRankerRecord() : m_pCache(NULL) { }

    virtual ~CGlobalRankerRecord();

    shared_ptr<sDBRequest> MakeInsertSQL() override;
    shared_ptr<sDBRequest> MakeUpdateSQL() override;
    shared_ptr<sDBRequest> MakeDeleteSQL() override;

    virtual int64_t GetUid();

    static std::vector<global_ranker_table_value_type> SelectDataFromDB(int64_t llUid, sDBSecRet& vecData);

    static const char* s_strFirstName;
    static const char* s_strSecName;
    static const char* s_strSecType;
    static const char* s_strRankerList;
    static const char* s_strResetTime;

    global_ranker_table_t* m_pCache;
    
    void SetRankerList(string strRankerList)
    {
        m_mapString[s_strRankerList] = strRankerList;
    }
    
    void SetResetTime(time_t llResetTime)
    {
        m_mapInt64[s_strResetTime] += llResetTime;
    }
    
    void SetCachePtr(global_ranker_table_t* pCache)
    {
        m_pCache = pCache;
    }
    void SetPrimaryKey(int64_t llUid)
    {
        m_llUid = llUid;
    }
private:
    int64_t m_llUid;
};

struct global_ranker_table_value_type
{
    global_ranker_table_value_type():
    m_llUid(0)
    , m_strRankerList("")
    , m_llResetTime(0)
    {}
    int64_t m_llUid;
    string m_strRankerList;
    time_t m_llResetTime;
};

struct global_ranker_table_t
{
    global_ranker_table_t() : m_pRecord(NULL) { }
    ~global_ranker_table_t() 
    {
        if(NULL != m_pRecord) m_pRecord->SetCachePtr(NULL);
    }

    void InitializeWithoutSQL(const global_ranker_table_value_type& stData)
    {
        m_stData = stData;
    }
    void Initialize(const global_ranker_table_value_type& stData)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::INSERT);
        m_pRecord->SetPrimaryKey(stData.m_llUid);
        m_pRecord->SetRankerList(stData.m_strRankerList);
        m_pRecord->SetResetTime(stData.m_llResetTime);
        m_stData = stData;
    }
    void ClearFromSQL()
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::REMOVE);
    }
    
    const string& GetRankerList() const
    {
        return m_stData.m_strRankerList;
    }
    
    void SetRankerList(const string& strRankerList)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetRankerList(strRankerList);
        m_stData.m_strRankerList = strRankerList;
    }
    
    const time_t& GetResetTime() const
    {
        return m_stData.m_llResetTime;
    }
    
    void SetResetTime(time_t llResetTime)
    {
        if (m_stData.m_llResetTime == llResetTime)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        time_t RealAdd = llResetTime - m_stData.m_llResetTime;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetResetTime(RealAdd);
        m_stData.m_llResetTime = llResetTime;
    }
    
    void AddResetTime(time_t llResetTime)
    {
        if (0 == llResetTime)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetResetTime(llResetTime);
        m_stData.m_llResetTime += llResetTime;
    }

    CGlobalRankerRecord* m_pRecord;
    global_ranker_table_value_type m_stData;

private:
    bool HasRecord()
    {
        return ((m_pRecord == NULL) ? false : true);
    }
    CGlobalRankerRecord* GetRecord()
    {
        if(!m_pRecord)
        {
            m_pRecord = new CGlobalRankerRecord;
            m_pRecord->SetPrimaryKey(m_stData.m_llUid);
            m_pRecord->SetCachePtr(this);
            SQL_TABLE_RECORD_CACHE_INS->m_stRecordVec.push_back(std::shared_ptr<CGlobalRankerRecord>(m_pRecord));
        }
        return m_pRecord;
    }
};