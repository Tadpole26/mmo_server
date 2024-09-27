#include "CTableRecordBase.h"
#include "log_mgr.h"

//sql����set,add��������,ͨ����β����ڴ桢�ı��ڴ����ݺ�,ͨ����һ���߳�����sqlִ��
/*
    1: �ϴ���insert,���delete,��Ϊundefined(û�п�ɾ���ļ�¼,������)
    2: �ϴ���insert,���update,��Ϊinsert(����һ�����ݼ���)
    3: �ϴ���update,�����delete,��Ϊdelete(����Ҫ�ı䣬ɾ�����ݼ���)
    4: �ϴ���update,�����insert,����
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
        //����ط�����������,insert->remove��UNDEFINED,��������
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

