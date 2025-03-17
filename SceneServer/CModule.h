#pragma once
#include "sdk.h"

class CGameUser;
//场景服模块组件基类
class CModule
{
public:
	CModule(uint32 uiType);
	virtual ~CModule();
	virtual void Final();
	virtual bool Init(CGameUser* pGameUser);
	virtual uint32 Serialize() = 0;
	virtual bool Unserialize() = 0;
public:
	inline uint32 GetType() const { return m_uiType; }
	inline CGameUser* GameUserPtr() { return m_pGameUser; }
	inline CGameUser& GameUserRef() { return *m_pGameUser; }
	void SaveDelay(uint32 uiEffect = 0);
protected:
	CGameUser* m_pGameUser = nullptr;
private:
	uint32 m_uiType = 0;
};
