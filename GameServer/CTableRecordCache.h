#pragma once

#include "CTableRecordBase.h"
#include "singleton.h"

#include <vector>
#include <string>
#include <memory>

class CTableRecordCache
{
public:
    void ExecSQL();

    std::vector<std::shared_ptr<CTableRecordBase>> m_stRecordVec;
};

#define SQL_TABLE_RECORD_CACHE_INS Singleton<CTableRecordCache>::getInstance()



