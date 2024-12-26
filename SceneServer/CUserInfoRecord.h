
#pragma once
#include "CTableRecordBase.h"
#include "CTableRecordCache.h"

struct user_info_table_t;
struct user_info_table_value_type;
class CUserInfoRecord : public CTableRecordBase
{
public:
    CUserInfoRecord() : m_pCache(NULL) { }

    virtual ~CUserInfoRecord();

    shared_ptr<sDBRequest> MakeInsertSQL() override;
    shared_ptr<sDBRequest> MakeUpdateSQL() override;
    shared_ptr<sDBRequest> MakeDeleteSQL() override;

    virtual int64_t GetUid();

    static std::vector<user_info_table_value_type> SelectDataFromDB(int64_t llUid, sDBSecRet& vecData);

    static const char* s_strFirstName;
    static const char* s_strSecName;
    static const char* s_strSecType;
    static const char* s_strUserName;
    static const char* s_strUserLevel;
    static const char* s_strUserExp;
    static const char* s_strCreateTime;
    static const char* s_strVipLevel;
    static const char* s_strVipExp;
    static const char* s_strInnerMailAddId;

    user_info_table_t* m_pCache;
    
    void SetUserName(string strUserName)
    {
        m_mapString[s_strUserName] = strUserName;
    }
    
    void SetUserLevel(int iUserLevel)
    {
        m_mapInt[s_strUserLevel] += iUserLevel;
    }
    
    void SetUserExp(int iUserExp)
    {
        m_mapInt[s_strUserExp] += iUserExp;
    }
    
    void SetCreateTime(time_t llCreateTime)
    {
        m_mapInt64[s_strCreateTime] += llCreateTime;
    }
    
    void SetVipLevel(int iVipLevel)
    {
        m_mapInt[s_strVipLevel] += iVipLevel;
    }
    
    void SetVipExp(int iVipExp)
    {
        m_mapInt[s_strVipExp] += iVipExp;
    }
    
    void SetInnerMailAddId(int64_t llInnerMailAddId)
    {
        m_mapInt64[s_strInnerMailAddId] += llInnerMailAddId;
    }
    
    void SetCachePtr(user_info_table_t* pCache)
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

struct user_info_table_value_type
{
    user_info_table_value_type():
    m_llUid(0)
    , m_strUserName("")
    , m_iUserLevel(0)
    , m_iUserExp(0)
    , m_llCreateTime(0)
    , m_iVipLevel(0)
    , m_iVipExp(0)
    , m_llInnerMailAddId(0)
    {}
    int64_t m_llUid;
    string m_strUserName;
    int m_iUserLevel;
    int m_iUserExp;
    time_t m_llCreateTime;
    int m_iVipLevel;
    int m_iVipExp;
    int64_t m_llInnerMailAddId;
};

struct user_info_table_t
{
    user_info_table_t() : m_pRecord(NULL) { }
    ~user_info_table_t() 
    {
        if(NULL != m_pRecord) m_pRecord->SetCachePtr(NULL);
    }

    void InitializeWithoutSQL(const user_info_table_value_type& stData)
    {
        m_stData = stData;
    }
    void Initialize(const user_info_table_value_type& stData)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::INSERT);
        m_pRecord->SetPrimaryKey(stData.m_llUid);
        m_pRecord->SetUserName(stData.m_strUserName);
        m_pRecord->SetUserLevel(stData.m_iUserLevel);
        m_pRecord->SetUserExp(stData.m_iUserExp);
        m_pRecord->SetCreateTime(stData.m_llCreateTime);
        m_pRecord->SetVipLevel(stData.m_iVipLevel);
        m_pRecord->SetVipExp(stData.m_iVipExp);
        m_pRecord->SetInnerMailAddId(stData.m_llInnerMailAddId);
        m_stData = stData;
    }
    void ClearFromSQL()
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::REMOVE);
    }
    
    const string& GetUserName() const
    {
        return m_stData.m_strUserName;
    }
    
    void SetUserName(const string& strUserName)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetUserName(strUserName);
        m_stData.m_strUserName = strUserName;
    }
    
    const int& GetUserLevel() const
    {
        return m_stData.m_iUserLevel;
    }
    
    void SetUserLevel(int iUserLevel)
    {
        if (m_stData.m_iUserLevel == iUserLevel)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int RealAdd = iUserLevel - m_stData.m_iUserLevel;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetUserLevel(RealAdd);
        m_stData.m_iUserLevel = iUserLevel;
    }
    
    void AddUserLevel(int iUserLevel)
    {
        if (0 == iUserLevel)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetUserLevel(iUserLevel);
        m_stData.m_iUserLevel += iUserLevel;
    }
    
    const int& GetUserExp() const
    {
        return m_stData.m_iUserExp;
    }
    
    void SetUserExp(int iUserExp)
    {
        if (m_stData.m_iUserExp == iUserExp)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int RealAdd = iUserExp - m_stData.m_iUserExp;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetUserExp(RealAdd);
        m_stData.m_iUserExp = iUserExp;
    }
    
    void AddUserExp(int iUserExp)
    {
        if (0 == iUserExp)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetUserExp(iUserExp);
        m_stData.m_iUserExp += iUserExp;
    }
    
    const time_t& GetCreateTime() const
    {
        return m_stData.m_llCreateTime;
    }
    
    void SetCreateTime(time_t llCreateTime)
    {
        if (m_stData.m_llCreateTime == llCreateTime)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        time_t RealAdd = llCreateTime - m_stData.m_llCreateTime;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetCreateTime(RealAdd);
        m_stData.m_llCreateTime = llCreateTime;
    }
    
    void AddCreateTime(time_t llCreateTime)
    {
        if (0 == llCreateTime)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetCreateTime(llCreateTime);
        m_stData.m_llCreateTime += llCreateTime;
    }
    
    const int& GetVipLevel() const
    {
        return m_stData.m_iVipLevel;
    }
    
    void SetVipLevel(int iVipLevel)
    {
        if (m_stData.m_iVipLevel == iVipLevel)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int RealAdd = iVipLevel - m_stData.m_iVipLevel;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetVipLevel(RealAdd);
        m_stData.m_iVipLevel = iVipLevel;
    }
    
    void AddVipLevel(int iVipLevel)
    {
        if (0 == iVipLevel)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetVipLevel(iVipLevel);
        m_stData.m_iVipLevel += iVipLevel;
    }
    
    const int& GetVipExp() const
    {
        return m_stData.m_iVipExp;
    }
    
    void SetVipExp(int iVipExp)
    {
        if (m_stData.m_iVipExp == iVipExp)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int RealAdd = iVipExp - m_stData.m_iVipExp;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetVipExp(RealAdd);
        m_stData.m_iVipExp = iVipExp;
    }
    
    void AddVipExp(int iVipExp)
    {
        if (0 == iVipExp)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetVipExp(iVipExp);
        m_stData.m_iVipExp += iVipExp;
    }
    
    const int64_t& GetInnerMailAddId() const
    {
        return m_stData.m_llInnerMailAddId;
    }
    
    void SetInnerMailAddId(int64_t llInnerMailAddId)
    {
        if (m_stData.m_llInnerMailAddId == llInnerMailAddId)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int64_t RealAdd = llInnerMailAddId - m_stData.m_llInnerMailAddId;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetInnerMailAddId(RealAdd);
        m_stData.m_llInnerMailAddId = llInnerMailAddId;
    }
    
    void AddInnerMailAddId(int64_t llInnerMailAddId)
    {
        if (0 == llInnerMailAddId)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetInnerMailAddId(llInnerMailAddId);
        m_stData.m_llInnerMailAddId += llInnerMailAddId;
    }

    CUserInfoRecord* m_pRecord;
    user_info_table_value_type m_stData;

private:
    bool HasRecord()
    {
        return ((m_pRecord == NULL) ? false : true);
    }
    CUserInfoRecord* GetRecord()
    {
        if(!m_pRecord)
        {
            m_pRecord = new CUserInfoRecord;
            m_pRecord->SetPrimaryKey(m_stData.m_llUid);
            m_pRecord->SetCachePtr(this);
            SQL_TABLE_RECORD_CACHE_INS->m_stRecordVec.push_back(std::shared_ptr<CUserInfoRecord>(m_pRecord));
        }
        return m_pRecord;
    }
};