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
	cServerInfoMgr* _mgr = nullptr;				//关联服信息
	cProcessInfo* _ptrsvr = nullptr;			//当前服信息
};

