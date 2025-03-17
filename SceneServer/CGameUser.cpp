#include "CGameUser.h"
#include "CModule.h"
#include "module.pb.h"

CGameUser::CGameUser(const uint64 ullRoleId) :
	CUser(ullRoleId),
	m_vecModuleMgr(client::enModule_End, nullptr)
{
}

CGameUser::~CGameUser()
{
}

void CGameUser::Final()
{
	//移除usermgr角色指针
	//释放所有模块数据
	for (auto iter = m_vecModuleMgr.begin(); iter != m_vecModuleMgr.end();)
	{
		auto del = iter++;
		auto* pModule = *del;
		if (pModule == nullptr) continue;
		pModule->Final();
		SAFE_DELETE(pModule);
	}
	m_vecModuleMgr.clear();
}

bool CGameUser::Init()
{
	for (auto iter = m_vecModuleMgr.begin(); iter != m_vecModuleMgr.end(); ++iter)
	{
		auto* pModule = *iter;
		if (pModule == nullptr) continue;
		if (!pModule->Init(this))
			return false;
	}
	return true;
}

CModule& CGameUser::__RegisterModule(CModule* pModule)
{
	assert(pModule != nullptr);
	assert(pModule->GetType() > 0 && pModule->GetType() < client::enModule_End);
	assert(m_vecModuleMgr[pModule->GetType()] == nullptr);
	m_vecModuleMgr[pModule->GetType()] = pModule;
	return *pModule;
}

CModule* CGameUser::__GetModule(const uint32 uiType)
{
	assert(uiType > 0 && uiType < client::enModule_End);
	return m_vecModuleMgr[uiType];
}