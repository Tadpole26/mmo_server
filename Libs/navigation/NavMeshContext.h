#pragma once
#include "sdk.h"

static const int MAX_POLYS = 256;
static const int MAX_SMOOTH = 2048;

class dtNavMesh;
class dtNavMeshQuery;
class MovementAgent;
class NavMeshContext
{
public:
	NavMeshContext();
	~NavMeshContext();
	bool load(const char* path);
	void clear();

	bool calculatePath(MovementAgent *agent, float destX, float destY, float destZ);
	std::list<zPos> getPath(MovementAgent* agent);
	float getPosPolyHeight(MovementAgent* agent) const;
	bool getRandomPoint(MovementAgent* agent, float radius, float& outX, float &outY, float &outZ) const;
/*

	dtStatus Raycast(const float* spos, const float* epos);

	float* fixPosition(const float* pos);
	float* GetHitPosition() { return m_hitPos; }
	*/
private:
	dtNavMesh* _navMesh;
	dtNavMeshQuery* _navQuery;
	float m_polyPickExt[3] = { 2,40,2 };

	uint32 m_startRef, m_endRef;				//¶à±ßÐÎË÷Òý
	float m_spos[3];
	float m_epos[3];

	uint32 m_polys[MAX_POLYS];
	int m_npolys = 0;

	float m_smoothPath[MAX_SMOOTH * 3];
	int m_nsmoothPath;
/*
	float m_fixPos[3];
	float m_hitPos[3];
	*/
};
