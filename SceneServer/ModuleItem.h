#pragma once
#include "GameModule.h"

class ItemBase
{
public:
	ItemBase();
	virtual ~ItemBase();
	virtual void final();
	virtual bool init();
private:
	uint32 _tid = 0;
	uint32 _count = 0;
};

class ItemEquip : public ItemBase
{
public:
	ItemEquip();
	~ItemEquip();
	void final();
	bool init();
private:
	uint32 _uid = 0;

};

class ModuleItem : public GameModule
{
public:
	//ModuleItem();
	//virtual ~ModuleItem();
	virtual void final() final;
	virtual bool init(GameUser* pUser) final;
};