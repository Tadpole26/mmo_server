#pragma once

#include <map>
#include <memory>
#include <string>
#include <inttypes.h>
#include "db_struct.h"
#include "mongo_base.h"

using namespace std;
using namespace bsoncxx::builder::stream;

class CTableRecordBase
{
public:
    CTableRecordBase() : m_eOperation(UNDEFINED) { }

    virtual ~CTableRecordBase() { }

    enum sql_operation_t
    {
        UNDEFINED = 0,
        INSERT = 1,
        UPDATE = 2,
        REMOVE = 3,
    };

    virtual shared_ptr<sDBRequest> MakeInsertSQL() = 0;
    virtual shared_ptr<sDBRequest> MakeUpdateSQL() = 0;
    virtual shared_ptr<sDBRequest> MakeDeleteSQL() = 0;
    std::shared_ptr<sDBRequest> MakeSQL();
    void SetOperation(sql_operation_t eOperation);
    //��ȡΨһid,���ﲻ�ô��麯��,ȫ�ֱ�Ψһkey��һ����int����
    virtual int64_t GetUid() { return 0; };

protected:
    sql_operation_t m_eOperation;
    //����̳���ЩMap,ÿ��set���Ǹı��ڴ�mapֵ
    std::map<const char*, bool>                 m_mapBool;
    std::map<const char*, int>                  m_mapInt;
    std::map<const char*, uint32_t>             m_mapUint32;
    std::map<const char*, int64_t>              m_mapInt64;
    std::map<const char*, int64_t>              m_mapTime;
    std::map<const char*, std::string>          m_mapString;
    std::map<const char*, std::string>          m_mapVarData;
};
