#pragma once

#include "sdk.h"

class cProcessInfo;
class cServerInfoMgr;
class cNetService
{
public:
	cNetService();
	virtual ~cNetService();
protected:
	bool init();
	virtual void final();
private:
	cServerInfoMgr* _mgr = nullptr;				//��������Ϣ
	cProcessInfo* _ptrsvr = nullptr;			//��ǰ����Ϣ
};

