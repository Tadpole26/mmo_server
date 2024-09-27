#include "CDbInstance.h"
#include "CGameLogic.h"
#include "log_mgr.h"

CDbInstance::CDbInstance()
{
}


CDbInstance::~CDbInstance()
{
}

bool CDbInstance::Init()
{
	bool bRet = false;

	for (size_t i = 0; i < DB_THREAD_NUM; ++i)
	{
		bRet = m_oQueryThread[i].Init(GAME_LOGIC_INS->m_oConstCfg.m_strMongoHost
		, GAME_LOGIC_INS->m_oConstCfg.m_strMongoUser, GAME_LOGIC_INS->m_oConstCfg.m_strMongoPasswd
		, GAME_LOGIC_INS->m_oConstCfg.m_strMongoGameDb, GAME_LOGIC_INS->m_oConstCfg.m_strMongoAuth);
		if (!bRet) return false;
	}

	bRet = m_oQuery.Init(GAME_LOGIC_INS->m_oConstCfg.m_strMongoHost
		, GAME_LOGIC_INS->m_oConstCfg.m_strMongoUser, GAME_LOGIC_INS->m_oConstCfg.m_strMongoPasswd
		, GAME_LOGIC_INS->m_oConstCfg.m_strMongoGameDb, GAME_LOGIC_INS->m_oConstCfg.m_strMongoAuth);

	if (!bRet) return false;

	bRet = m_oMailQuery.Init(GAME_LOGIC_INS->m_oConstCfg.m_strMongoHost
		, GAME_LOGIC_INS->m_oConstCfg.m_strMongoUser, GAME_LOGIC_INS->m_oConstCfg.m_strMongoPasswd
		, GAME_LOGIC_INS->m_oConstCfg.m_strMongoMailDb, GAME_LOGIC_INS->m_oConstCfg.m_strMongoAuth, false);

	if (!bRet) return false;

	return true;
}

void CDbInstance::UnInit()
{
	for (size_t i = 0; i < DB_THREAD_NUM; i++)
	{
		m_oQueryThread[i].UnInit();
	}
}

void CDbInstance::OnTimer()
{
	static uint32 uiCount = 0;
	if (++uiCount >= 30)
	{
		m_oQuery.DbPing();
		m_oMailQuery.DbPing();
		uiCount = 0;
	}

	static uint32 uiFailedTimer = 0;
	if (++uiFailedTimer >= 5)
	{
		uint32 uiFailedNum = 0;
		for (size_t i = 0; i < DB_THREAD_NUM; ++i)
		{
			uiFailedNum += m_oQueryThread[i].GetFailedNum();
		}

		if (uiFailedNum >= 5)
		{
			//¶à´ÎÊ§°Ü¶ÏµôÍø¹Ø
		}

		uiFailedTimer = 0;
	}
}

bool CDbInstance::QueryData(sDBSelectReq& stRequest)
{
	return m_oQuery.DBSelectOne(stRequest);
}

bool CDbInstance::QueryMultiData(sDBSelectReq& stRequest, fn_RetCall fnCall, size_t& uiRetNum
	, int64 llBeginId, int32 iNum)
{
	return m_oQuery.DBSelectMore(stRequest, fnCall, uiRetNum, llBeginId, iNum);
}

int64 CDbInstance::QueryCount(sDBSelectReq& stRequest)
{
	return m_oQuery.DBCount(stRequest);
}

void CDbInstance::PushGame(sDBRequest& stRequest)
{
	m_oQueryThread[(stRequest.Con().NumULong("_id") / INDEX_MOD_NUM) % DB_THREAD_NUM].PushData(stRequest);
}

bool CDbInstance::Exec(sDBRequest& stRequest)
{
	return m_oQuery.DBExec(stRequest);
}
