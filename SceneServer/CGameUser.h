#pragma once
#include "sdk.h"
#include "CUser.h"

class CSceneScene;
class CModule;
class CGameUser : public CUser
{
public:
	CGameUser(const uint64 ullRoleId);
private:
	virtual ~CGameUser();
	virtual void Final();
private:
	using ModuleVecT = std::vector<CModule*>;
private:
	ModuleVecT m_vecModuleMgr;
private:
	CModule& __RegisterModule(CModule* pModule);
	CModule* __GetModule(const uint32 uiType);
public:
	bool Init();
	void SaveNow();
	void SaveDelay(CModule* pModule, uint32 uiEffect);
public:
	//∫√”—

public:
	uint32 m_uiGateId = 0;
	CSceneScene* m_pScene = nullptr;
};
