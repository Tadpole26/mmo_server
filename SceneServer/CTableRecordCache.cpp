#include "CTableRecordCache.h"
#include "CDbInstance.h"

void CTableRecordCache::ExecSQL()
{
	if (false == m_stRecordVec.empty())
	{
		std::vector<std::string> stSQL;

		for (auto& stIter : m_stRecordVec)
		{
			std::shared_ptr<sDBRequest> sql = stIter->MakeSQL();
			if (sql.get() != nullptr)
			{
				GAME_DB_INS->PushGame(*sql);
			}
			else
			{
				Log_Error("error db opt....");
			}
		}
		m_stRecordVec.clear();
	}
}