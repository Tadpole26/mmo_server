#pragma once
#include "GameModule.h"

class ModuleMatch : public GameModule
{
public:
	ModuleMatch();
	virtual ~ModuleMatch();
	virtual void final();
public:
	bool msgStartMatch(uint32 type);
	bool msgCancelMatch();
	bool msgConfirmOk();
	bool msgRefuseOk();
	bool msgCopyMatchOperationCancel();
public:
	bool enterPvpByVote(uint32 mtype);	//ͶƱ
private:
};