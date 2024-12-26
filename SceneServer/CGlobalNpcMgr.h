#pragma once
#include "id_alloctor.h"
#include "singleton.h"
#include <unordered_map>

class CNpc;
//��������NPC������(����)
class CGlobalNpcMgr
{
public:
	CGlobalNpcMgr();
	~CGlobalNpcMgr();
	void final();
public:
	uint32 AllocateUid();
	void FreeUid(uint32 uiUid);
	bool AddNpc(CNpc* pNpc);
	void RemoveNpc(CNpc* pNpc);
	CNpc* GetNpc(uint32 uiUid);
private:
	CIdAlloctor<> m_oIdAlloctor;
	std::unordered_map<uint32, CNpc*> m_mapNpcs;
};

#define gGlobalNpcMgr Singleton<CGlobalNpcMgr>::getInstance()