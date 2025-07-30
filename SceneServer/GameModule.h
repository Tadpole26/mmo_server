#pragma once
#include "sdk.h"

class GameUser;
//场景服模块组件基类
class GameModule
{
public:
	GameModule(uint32 type);
	virtual ~GameModule();
	virtual void final();
	virtual bool init(GameUser* pGameUser);
	virtual uint32 Serialize() = 0;
	virtual bool Unserialize() = 0;
public:
	inline uint32 getType() const { return _mtype; }
	inline GameUser* GameUserPtr() { return _pUser; }
	inline GameUser& GameUserRef() { return *_pUser; }
	void saveDelay(uint32 effect = 0);
protected:
	GameUser* _pUser = nullptr;
private:
	uint32 _mtype = 0;
};
