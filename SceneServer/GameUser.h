#pragma once
#include "sdk.h"
#include "cUser.h"

class CSceneScene;
class GameModule;
class GameUser : public cUser
{
public:
	GameUser(const uint64 ullRoleId);
	virtual ~GameUser();
	virtual void final();
private:
	using ModuleVecT = std::vector<GameModule*>;
private:
	ModuleVecT _moduleMgr;
public:
	bool init();
	void saveNow();
	void saveDelay(GameModule* pModule, uint32 uiEffect);
	void saveForce();
public:
	GameModule& registerModule(GameModule* pModule);
	GameModule* getModule(const uint32 uiType);
public:
	uint32 gateId = 0;
	CSceneScene* scene = nullptr;
};
