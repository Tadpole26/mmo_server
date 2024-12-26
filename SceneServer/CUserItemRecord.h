
#pragma once
#include "CTableRecordBase.h"
#include "CTableRecordCache.h"

struct user_item_table_t;
struct user_item_table_value_type;
class CUserItemRecord : public CTableRecordBase
{
public:
    CUserItemRecord() : m_pCache(NULL) { }

    virtual ~CUserItemRecord();

    shared_ptr<sDBRequest> MakeInsertSQL() override;
    shared_ptr<sDBRequest> MakeUpdateSQL() override;
    shared_ptr<sDBRequest> MakeDeleteSQL() override;

    virtual int64_t GetUid();
    virtual int GetId();

    static std::vector<user_item_table_value_type> SelectDataFromDB(int64_t llUid, sDBSecRet& vecData);

    static const char* s_strFirstName;
    static const char* s_strSecName;
    static const char* s_strSecType;
    static const char* s_strId;
    static const char* s_strItemNum;
    static const char* s_strItemTime;

    user_item_table_t* m_pCache;
    
    void SetId(int iId)
    {
        m_mapInt[s_strId] += iId;
    }
    
    void SetItemNum(int iItemNum)
    {
        m_mapInt[s_strItemNum] += iItemNum;
    }
    
    void SetItemTime(time_t llItemTime)
    {
        m_mapInt64[s_strItemTime] += llItemTime;
    }
    
    void SetCachePtr(user_item_table_t* pCache)
    {
        m_pCache = pCache;
    }
    void SetPrimaryKey(int64_t llUid, int iId)
    {
        m_llUid = llUid;
        m_iId = iId;
    }
private:
    int64_t m_llUid;
    int m_iId;
};

struct user_item_table_value_type
{
    user_item_table_value_type():
    m_llUid(0)
    , m_iId(0)
    , m_iItemNum(0)
    , m_llItemTime(0)
    {}
    int64_t m_llUid;
    int m_iId;
    int m_iItemNum;
    time_t m_llItemTime;
};

struct user_item_table_t
{
    user_item_table_t() : m_pRecord(NULL) { }
    ~user_item_table_t() 
    {
        if(NULL != m_pRecord) m_pRecord->SetCachePtr(NULL);
    }

    void InitializeWithoutSQL(const user_item_table_value_type& stData)
    {
        m_stData = stData;
    }
    void Initialize(const user_item_table_value_type& stData)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::INSERT);
        m_pRecord->SetPrimaryKey(stData.m_llUid, stData.m_iId);
        m_pRecord->SetId(stData.m_iId);
        m_pRecord->SetItemNum(stData.m_iItemNum);
        m_pRecord->SetItemTime(stData.m_llItemTime);
        m_stData = stData;
    }
    void ClearFromSQL()
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::REMOVE);
    }
    
    const int& GetId() const
    {
        return m_stData.m_iId;
    }
    
    void SetId(int iId)
    {
        if (m_stData.m_iId == iId)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int RealAdd = iId - m_stData.m_iId;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetId(RealAdd);
        m_stData.m_iId = iId;
    }
    
    void AddId(int iId)
    {
        if (0 == iId)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetId(iId);
        m_stData.m_iId += iId;
    }
    
    const int& GetItemNum() const
    {
        return m_stData.m_iItemNum;
    }
    
    void SetItemNum(int iItemNum)
    {
        if (m_stData.m_iItemNum == iItemNum)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int RealAdd = iItemNum - m_stData.m_iItemNum;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetItemNum(RealAdd);
        m_stData.m_iItemNum = iItemNum;
    }
    
    void AddItemNum(int iItemNum)
    {
        if (0 == iItemNum)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetItemNum(iItemNum);
        m_stData.m_iItemNum += iItemNum;
    }
    
    const time_t& GetItemTime() const
    {
        return m_stData.m_llItemTime;
    }
    
    void SetItemTime(time_t llItemTime)
    {
        if (m_stData.m_llItemTime == llItemTime)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        time_t RealAdd = llItemTime - m_stData.m_llItemTime;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetItemTime(RealAdd);
        m_stData.m_llItemTime = llItemTime;
    }
    
    void AddItemTime(time_t llItemTime)
    {
        if (0 == llItemTime)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetItemTime(llItemTime);
        m_stData.m_llItemTime += llItemTime;
    }

    CUserItemRecord* m_pRecord;
    user_item_table_value_type m_stData;

private:
    bool HasRecord()
    {
        return ((m_pRecord == NULL) ? false : true);
    }
    CUserItemRecord* GetRecord()
    {
        if(!m_pRecord)
        {
            m_pRecord = new CUserItemRecord;
            m_pRecord->SetPrimaryKey(m_stData.m_llUid, m_stData.m_iId);
            m_pRecord->SetCachePtr(this);
            SQL_TABLE_RECORD_CACHE_INS->m_stRecordVec.push_back(std::shared_ptr<CUserItemRecord>(m_pRecord));
        }
        return m_pRecord;
    }
};