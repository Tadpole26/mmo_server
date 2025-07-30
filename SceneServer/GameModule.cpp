#include "GameModule.h"
#include "GameUser.h"

GameModule::GameModule(uint32 type) : _mtype(type)
{
}

GameModule::~GameModule()
{
}

void GameModule::final()
{
	_pUser = nullptr;
}

bool GameModule::init(GameUser* pGameUser)
{
	assert(pGameUser != nullptr);
	assert(_pUser == nullptr);
	_pUser = pGameUser;
	return true;
}

void GameModule::saveDelay(uint32 uiEffect)
{
	_pUser->saveDelay(this, uiEffect);
}