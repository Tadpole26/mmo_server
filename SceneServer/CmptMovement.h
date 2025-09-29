#pragma once
#include "MovementAgent.h"

class CmptMovement : public MovementAgent
{
public:
	CmptMovement();
	~CmptMovement();
	void final();
public:
	virtual void getPos(float& x, float& y, float& z) const;
	virtual const NavMeshContext* getNavMeshContext() const;	
	virtual bool canWalk() const;
private:

};
