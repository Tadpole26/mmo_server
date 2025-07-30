#pragma once
#include "sdk.h"
#include "mongo_db.h"
#include "mongo_mgr.h"
#include "singleton.h"
#include "CDbInstance.h"

class CDbInstance
{
public:
	CDbInstance();
	~CDbInstance();

	bool Init();
	void UnInit();
	void OnTimer();

	bool QueryData(sDBSelectReq& stRequest);
	bool QueryMultiData(sDBSelectReq& stRequest, fn_RetCall fnCall, size_t& uiRetNum
		, int64 llBeginId, int32 iNum);
	int64 QueryCount(sDBSelectReq& stRequest);

	void PushGame(sDBRequest& stRequest);
	bool Exec(sDBRequest& stRequest);

public:
	MongoDb m_oQuery;										//ͬ�������������mongo
	MongoMgr m_oQueryThread[DB_THREAD_NUM];					//�첽�����������mongo
	MongoDb m_oMailQuery;									//ͬ����ȡȫ���ʼ�
};

#define GAME_DB_INS Singleton<CDbInstance>::getInstance()
