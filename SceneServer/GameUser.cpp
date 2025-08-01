#include "GameUser.h"
#include "GameModule.h"
#include "client.pb.h"

GameUser::GameUser(const uint64 ullRoleId) :
	cUser(ullRoleId),
	_moduleMgr(client::enModule_End, nullptr)
{
}

GameUser::~GameUser()
{
}

void GameUser::final()
{
	//�Ƴ�usermgr��ɫָ��
	//�ͷ�����ģ������
	for (auto iter = _moduleMgr.begin(); iter != _moduleMgr.end();)
	{
		auto del = iter++;
		auto* pModule = *del;
		if (pModule == nullptr) continue;
		pModule->final();
		SAFE_DELETE(pModule);
	}
	_moduleMgr.clear();
}

bool GameUser::init()
{
	for (auto iter = _moduleMgr.begin(); iter != _moduleMgr.end(); ++iter)
	{
		auto* pModule = *iter;
		if (pModule == nullptr) continue;
		if (!pModule->init(this))
			return false;
	}
	return true;
}

GameModule& GameUser::__registerModule(GameModule* pModule)
{
	assert(pModule != nullptr);
	assert(pModule->getType() > 0 && pModule->getType() < client::enModule_End);
	assert(_moduleMgr[pModule->getType()] == nullptr);
	_moduleMgr[pModule->getType()] = pModule;
	return *pModule;
}

GameModule* GameUser::__getModule(const uint32 uiType)
{
	assert(uiType > 0 && uiType < client::enModule_End);
	return _moduleMgr[uiType];
}

void GameUser::saveNow()
{
}

void GameUser::saveDelay(GameModule* pModule, uint32 uiEffect)
{
}