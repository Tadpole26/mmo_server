#include "CTableRecordBase.h"
#include "log_mgr.h"

//sql不会set,add立刻生成,通过多次操作内存、改变内存数据后,通过另一个线程生成sql执行
/*
    1: 上次是insert,这次delete,变为undefined(没有可删除的记录,不操作)
    2: 上次是insert,这次update,变为insert(插入一条数据即可)
    3: 上次是update,这次是delete,变为delete(不需要改变，删除数据即可)
    4: 上次是update,这次是insert,错误
*/
void CTableRecordBase::SetOperation(sql_operation_t eOperation)
{
    if (m_eOperation == eOperation)
        return;

    switch (m_eOperation)
    {
    case UNDEFINED:
        m_eOperation = eOperation;
        break;
    case INSERT:
        if (eOperation == REMOVE)
        {
            m_eOperation = UNDEFINED;
        }
        else if (eOperation == UPDATE)
        {
            m_eOperation = INSERT;
        }
        break;
    case UPDATE:
        if (eOperation == REMOVE)
        {
            m_eOperation = REMOVE;
        }
        else if (eOperation == INSERT)
        {
            Log_Error("operate error, insert ater update");
            break;
        }
        break;
    case REMOVE:
        break;
    }
}

shared_ptr<sDBRequest> CTableRecordBase::MakeSQL()
{
    switch (m_eOperation)
    {
        //这个地方可能有问题,insert->remove是UNDEFINED,属于正常
    case UNDEFINED:
        return nullptr;
    case INSERT:
        return MakeInsertSQL();
    case UPDATE:
        return MakeUpdateSQL();
    case REMOVE:
        return MakeDeleteSQL();
    default:
        return nullptr;
    }
}

