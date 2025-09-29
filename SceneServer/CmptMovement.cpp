#include "CmptMovement.h"

CmptMovement::CmptMovement() : MovementAgent()
{
}

CmptMovement::~CmptMovement()
{
}

void CmptMovement::final()
{
	MovementAgent::final();
}

void CmptMovement::getPos(float& x, float& y, float& z) const
{
	x = 10.13;
	y = -2.3624;
	z = 12.91;
}

const NavMeshContext* CmptMovement::getNavMeshContext() const
{
	return nullptr;

}

bool CmptMovement::canWalk() const
{
	return true;
}