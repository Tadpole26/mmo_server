#include "CModule.h"
#include "CGameUser.h"

CModule::CModule(uint32 uiType) : m_uiType(uiType)
{
}

CModule::~CModule()
{
}

void CModule::Final()
{
	m_pGameUser = nullptr;
}

bool CModule::Init(CGameUser* pGameUser)
{
	assert(pGameUser != nullptr);
	assert(m_pGameUser == nullptr);
	m_pGameUser = pGameUser;
	return true;
}

void CModule::SaveDelay(uint32 uiEffect)
{
	m_pGameUser->SaveDelay(this, uiEffect);
}