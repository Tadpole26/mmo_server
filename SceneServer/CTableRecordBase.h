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
    //获取唯一id,这里不用纯虚函数,全局表唯一key不一定是int类型
    virtual int64_t GetUid() { return 0; };

protected:
    sql_operation_t m_eOperation;
    //子类继承这些Map,每次set都是改变内存map值
    std::map<const char*, bool>                 m_mapBool;
    std::map<const char*, int>                  m_mapInt;
    std::map<const char*, uint32_t>             m_mapUint32;
    std::map<const char*, int64_t>              m_mapInt64;
    std::map<const char*, int64_t>              m_mapTime;
    std::map<const char*, std::string>          m_mapString;
    std::map<const char*, std::string>          m_mapVarData;
};
