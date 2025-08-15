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
	virtual uint32 serialize() = 0;
	virtual bool unserialize() = 0;
public:
	virtual bool handleClientMsg(uint32 cmdId, const std::string& data) = 0;
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
