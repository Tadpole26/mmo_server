#pragma once
#include <unordered_map>

class CNpc;
//场景单类NPC管理基类
class CNpcMgr
{
public:
	CNpcMgr();
	virtual ~CNpcMgr();
public:
	virtual bool init() = 0;
	virtual void final();
protected:
	virtual uint32 _allocUid();
	virtual bool _addNpc(CNpc* pNpc);
	virtual void _removeNpc(CNpc* pNpc);
private:
	std::unordered_map<uint32, CNpc*> m_mapNpcs;
};
