#include "ztype.h"
#include "CNpc.h"
#include "CGlobalNpcMgr.h"

CGlobalNpcMgr::CGlobalNpcMgr()
{
}

CGlobalNpcMgr::~CGlobalNpcMgr()
{
}

void CGlobalNpcMgr::final()
{
}

uint32 CGlobalNpcMgr::AllocateUid()
{
	auto id = m_oIdAlloctor.alloc();
	if (!m_oIdAlloctor.check(id)) return 0;
	return id;
}

void CGlobalNpcMgr::FreeUid(uint32 uiUid)
{
	m_oIdAlloctor.free(uiUid);
}

bool CGlobalNpcMgr::AddNpc(CNpc* pNpc)
{
	auto ret = m_mapNpcs.emplace(pNpc->GetUid(), pNpc);
	if (!ret.second)
		return false;
	//if (pNpc)
	return true;
}

void CGlobalNpcMgr::RemoveNpc(CNpc* pNpc)
{
	m_mapNpcs.erase(pNpc->GetUid());
}

CNpc* CGlobalNpcMgr::GetNpc(uint32 uiUid)
{
	auto it = m_mapNpcs.find(uiUid);
	if (it == m_mapNpcs.end()) return nullptr;
	return it->second;
}