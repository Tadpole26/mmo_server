
#pragma once
#include "CTableRecordBase.h"
#include "CTableRecordCache.h"

struct user_card_table_t;
struct user_card_table_value_type;
class CUserCardRecord : public CTableRecordBase
{
public:
    CUserCardRecord() : m_pCache(NULL) { }

    virtual ~CUserCardRecord();

    shared_ptr<sDBRequest> MakeInsertSQL() override;
    shared_ptr<sDBRequest> MakeUpdateSQL() override;
    shared_ptr<sDBRequest> MakeDeleteSQL() override;

    virtual int64_t GetUid();
    virtual int GetId();

    static std::vector<user_card_table_value_type> SelectDataFromDB(int64_t llUid, sDBSecRet& vecData);

    static const char* s_strFirstName;
    static const char* s_strSecName;
    static const char* s_strSecType;
    static const char* s_strId;
    static const char* s_strCardLevel;
    static const char* s_strCardColor;
    static const char* s_strCardStar;

    user_card_table_t* m_pCache;
    
    void SetId(int iId)
    {
        m_mapInt[s_strId] += iId;
    }
    
    void SetCardLevel(int iCardLevel)
    {
        m_mapInt[s_strCardLevel] += iCardLevel;
    }
    
    void SetCardColor(int iCardColor)
    {
        m_mapInt[s_strCardColor] += iCardColor;
    }
    
    void SetCardStar(int iCardStar)
    {
        m_mapInt[s_strCardStar] += iCardStar;
    }
    
    void SetCachePtr(user_card_table_t* pCache)
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

struct user_card_table_value_type
{
    user_card_table_value_type():
    m_llUid(0)
    , m_iId(0)
    , m_iCardLevel(0)
    , m_iCardColor(0)
    , m_iCardStar(0)
    {}
    int64_t m_llUid;
    int m_iId;
    int m_iCardLevel;
    int m_iCardColor;
    int m_iCardStar;
};

struct user_card_table_t
{
    user_card_table_t() : m_pRecord(NULL) { }
    ~user_card_table_t() 
    {
        if(NULL != m_pRecord) m_pRecord->SetCachePtr(NULL);
    }

    void InitializeWithoutSQL(const user_card_table_value_type& stData)
    {
        m_stData = stData;
    }
    void Initialize(const user_card_table_value_type& stData)
    {
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::INSERT);
        m_pRecord->SetPrimaryKey(stData.m_llUid, stData.m_iId);
        m_pRecord->SetId(stData.m_iId);
        m_pRecord->SetCardLevel(stData.m_iCardLevel);
        m_pRecord->SetCardColor(stData.m_iCardColor);
        m_pRecord->SetCardStar(stData.m_iCardStar);
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
    
    const int& GetCardLevel() const
    {
        return m_stData.m_iCardLevel;
    }
    
    void SetCardLevel(int iCardLevel)
    {
        if (m_stData.m_iCardLevel == iCardLevel)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int RealAdd = iCardLevel - m_stData.m_iCardLevel;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetCardLevel(RealAdd);
        m_stData.m_iCardLevel = iCardLevel;
    }
    
    void AddCardLevel(int iCardLevel)
    {
        if (0 == iCardLevel)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetCardLevel(iCardLevel);
        m_stData.m_iCardLevel += iCardLevel;
    }
    
    const int& GetCardColor() const
    {
        return m_stData.m_iCardColor;
    }
    
    void SetCardColor(int iCardColor)
    {
        if (m_stData.m_iCardColor == iCardColor)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int RealAdd = iCardColor - m_stData.m_iCardColor;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetCardColor(RealAdd);
        m_stData.m_iCardColor = iCardColor;
    }
    
    void AddCardColor(int iCardColor)
    {
        if (0 == iCardColor)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetCardColor(iCardColor);
        m_stData.m_iCardColor += iCardColor;
    }
    
    const int& GetCardStar() const
    {
        return m_stData.m_iCardStar;
    }
    
    void SetCardStar(int iCardStar)
    {
        if (m_stData.m_iCardStar == iCardStar)
            return; 
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        int RealAdd = iCardStar - m_stData.m_iCardStar;
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetCardStar(RealAdd);
        m_stData.m_iCardStar = iCardStar;
    }
    
    void AddCardStar(int iCardStar)
    {
        if (0 == iCardStar)
            return;
        if (m_pRecord == NULL)
            m_pRecord = GetRecord();
        m_pRecord->SetOperation(CTableRecordBase::UPDATE);
        m_pRecord->SetCardStar(iCardStar);
        m_stData.m_iCardStar += iCardStar;
    }

    CUserCardRecord* m_pRecord;
    user_card_table_value_type m_stData;

private:
    bool HasRecord()
    {
        return ((m_pRecord == NULL) ? false : true);
    }
    CUserCardRecord* GetRecord()
    {
        if(!m_pRecord)
        {
            m_pRecord = new CUserCardRecord;
            m_pRecord->SetPrimaryKey(m_stData.m_llUid, m_stData.m_iId);
            m_pRecord->SetCachePtr(this);
            SQL_TABLE_RECORD_CACHE_INS->m_stRecordVec.push_back(std::shared_ptr<CUserCardRecord>(m_pRecord));
        }
        return m_pRecord;
    }
};