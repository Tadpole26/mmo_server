
#pragma once
#include "CTableRecordBase.h"
#include "CTableRecordCache.h"

struct user_mail_table_t;
struct user_mail_table_value_type;
class CUserMailRecord : public CTableRecordBase
{
public:
    CUserMailRecord() : m_pCache(NULL) { }

    virtual ~CUserMailRecord();

    shared_ptr<sDBRequest> MakeInsertSQL() override;
    shared_ptr<sDBRequest> MakeUpdateSQL() override;
    shared_ptr<sDBRequest> MakeDeleteSQL() override;

    virtual int64_t GetUid();
    virtual int64_t GetId();

    static std::vector<user_mail_table_value_type> SelectDataFromDB(int64_t llUid, sDBSecRet& vecData);

    static const char* s_strFirstName;
    static const char* s_strSecName;
    static const char* s_strSecType;
    static const char* s_strId;
    static const char* s_strMailType;
    static const char* s_strSendTime;
    static const char* s_strExpiredTime;
    static const char* s_strIsDelete;
    static const char* s_strIsReceived;
    static const char* s_strTitile;
    static const char* s_strSender;
    static const char* s_strContent;
    static const char* s_strAttachment;

    user_mail_table_t* m_pCache;
    
    void SetId(int64_t llId)
    {
        m_mapInt64[s_strId] += llId;
    }
    
    void SetMailType(int iMailType)
    {
        m_mapInt[s_strMailType] += iMailType;
    }
    
    void SetSendTime(time_t llSendTime)
    {
        m_mapInt64[s_strSendTime] += llSendTime;
    }
    
    void SetExpiredTime(time_t llExpiredTime)
    {
        m_mapInt64[s_strExpiredTime] += llExpiredTime;
    }
    
    void SetIsDelete(bool bIsDelete)
    {
        m_mapBool[s_strIsDelete] = bIsDelete;
    }
    
    void SetIsReceived(bool bIsReceived)
    {
        m_mapBool[s_strIsReceived] = bIsReceived;
    }
    
    void SetTitile(string strTitile)
    {
        m_mapString[s_strTitile] = strTitile;
    }
    
    void SetSender(string strSender)
    {
        m_mapString[s_strSender] = strSender;
    }
    
    void SetContent(string strContent)
    {
        m_mapString[s_strContent] = strContent;
    }
    
    void SetAttachment(string strAttachment)
    {
        m_mapString[s_strAttachment] = strAttachment;
    }
    
    void SetCachePtr(user_mail_table_t* pCache)
    {
        m_pCache = pCache;
    }
    void SetPrimaryKey(int64_t llUid, int64_t llId)
    {
        m_llUid = llUid;
        m_llId = llId;
    }
private:
    int64_t m_llUid;
    int64_t m_llId;
};

struct user_mail_table_value_type
{
    user_mail_table_value_type():
    m_llUid(0)
    , m_llId(0)
    , m_iMailType(0)
    , m_llSendTime(0)
    , m_llExpiredTime(0)
    , m_bIsDelete(false)
    , m_bIsReceived(false)
    , m_strTitile("")
    , m_strSender("")
    , m_strContent("")
    , m_strAttachment("")
    {}
    int64_t m_llUid;
    int64_t m_llId;
    int m_iMailType;
    time_t m_llSendTime;
    time_t m_llExpiredTime;
    bool m_bIsDelete;
    bool m_bIsReceived;
    string m_strTitile;
    string m_strSender;
    string m_strContent;
    string m_strAttachment;
};

struct user_mail_table_t
{
    user_mail_table_t() : m_pRecord(NULL) { }
    ~user_mail_table_t() 
    {
        if(NULL != m_pRecord) m_pRecord->SetCachePtr(NULL);
    }

    void InitializeWithoutSQL(const user_mail_table_value_type& stData)
    {
        m_stData = stData;
    }
    void Initialize(const user_mail_table_value_type& stData)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::INSERT);
        m_pRecord->SetPrimaryKey(stData.m_llUid, stData.m_llId);
        m_pRecord->SetId(stData.m_llId);
        m_pRecord->SetMailType(stData.m_iMailType);
        m_pRecord->SetSendTime(stData.m_llSendTime);
        m_pRecord->SetExpiredTime(stData.m_llExpiredTime);
        m_pRecord->SetIsDelete(stData.m_bIsDelete);
        m_pRecord->SetIsReceived(stData.m_bIsReceived);
        m_pRecord->SetTitile(stData.m_strTitile);
        m_pRecord->SetSender(stData.m_strSender);
        m_pRecord->SetContent(stData.m_strContent);
        m_pRecord->SetAttachment(stData.m_strAttachment);
        m_stData = stData;
    }
    void ClearFromSQL()
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::REMOVE);
    }
    
    const int64_t& GetId() const
    {
        return m_stData.m_llId;
    }
    
    void SetId(int64_t llId)
    {
        if (m_stData.m_llId == llId)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int64_t RealAdd = llId - m_stData.m_llId;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetId(RealAdd);
        m_stData.m_llId = llId;
    }
    
    void AddId(int64_t llId)
    {
        if (0 == llId)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetId(llId);
        m_stData.m_llId += llId;
    }
    
    const int& GetMailType() const
    {
        return m_stData.m_iMailType;
    }
    
    void SetMailType(int iMailType)
    {
        if (m_stData.m_iMailType == iMailType)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int RealAdd = iMailType - m_stData.m_iMailType;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetMailType(RealAdd);
        m_stData.m_iMailType = iMailType;
    }
    
    void AddMailType(int iMailType)
    {
        if (0 == iMailType)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetMailType(iMailType);
        m_stData.m_iMailType += iMailType;
    }
    
    const time_t& GetSendTime() const
    {
        return m_stData.m_llSendTime;
    }
    
    void SetSendTime(time_t llSendTime)
    {
        if (m_stData.m_llSendTime == llSendTime)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        time_t RealAdd = llSendTime - m_stData.m_llSendTime;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetSendTime(RealAdd);
        m_stData.m_llSendTime = llSendTime;
    }
    
    void AddSendTime(time_t llSendTime)
    {
        if (0 == llSendTime)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetSendTime(llSendTime);
        m_stData.m_llSendTime += llSendTime;
    }
    
    const time_t& GetExpiredTime() const
    {
        return m_stData.m_llExpiredTime;
    }
    
    void SetExpiredTime(time_t llExpiredTime)
    {
        if (m_stData.m_llExpiredTime == llExpiredTime)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        time_t RealAdd = llExpiredTime - m_stData.m_llExpiredTime;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetExpiredTime(RealAdd);
        m_stData.m_llExpiredTime = llExpiredTime;
    }
    
    void AddExpiredTime(time_t llExpiredTime)
    {
        if (0 == llExpiredTime)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetExpiredTime(llExpiredTime);
        m_stData.m_llExpiredTime += llExpiredTime;
    }
    
    const bool& GetIsDelete() const
    {
        return m_stData.m_bIsDelete;
    }
    
    void SetIsDelete(bool bIsDelete)
    {
        if (m_stData.m_bIsDelete == bIsDelete)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        bool RealAdd = bIsDelete - m_stData.m_bIsDelete;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetIsDelete(RealAdd);
        m_stData.m_bIsDelete = bIsDelete;
    }
    
    const bool& GetIsReceived() const
    {
        return m_stData.m_bIsReceived;
    }
    
    void SetIsReceived(bool bIsReceived)
    {
        if (m_stData.m_bIsReceived == bIsReceived)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        bool RealAdd = bIsReceived - m_stData.m_bIsReceived;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetIsReceived(RealAdd);
        m_stData.m_bIsReceived = bIsReceived;
    }
    
    const string& GetTitile() const
    {
        return m_stData.m_strTitile;
    }
    
    void SetTitile(const string& strTitile)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetTitile(strTitile);
        m_stData.m_strTitile = strTitile;
    }
    
    const string& GetSender() const
    {
        return m_stData.m_strSender;
    }
    
    void SetSender(const string& strSender)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetSender(strSender);
        m_stData.m_strSender = strSender;
    }
    
    const string& GetContent() const
    {
        return m_stData.m_strContent;
    }
    
    void SetContent(const string& strContent)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetContent(strContent);
        m_stData.m_strContent = strContent;
    }
    
    const string& GetAttachment() const
    {
        return m_stData.m_strAttachment;
    }
    
    void SetAttachment(const string& strAttachment)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetAttachment(strAttachment);
        m_stData.m_strAttachment = strAttachment;
    }

    CUserMailRecord* m_pRecord;
    user_mail_table_value_type m_stData;

private:
    bool HasRecord()
    {
        return ((m_pRecord == NULL) ? false : true);
    }
    CUserMailRecord* GetRecord()
    {
        if(!m_pRecord)
        {
            m_pRecord = new CUserMailRecord;
            m_pRecord->SetPrimaryKey(m_stData.m_llUid, m_stData.m_llId);
            m_pRecord->SetCachePtr(this);
            SQL_TABLE_RECORD_CACHE_INS->m_stRecordVec.push_back(std::shared_ptr<CUserMailRecord>(m_pRecord));
        }
        return m_pRecord;
    }
};