#pragma once

#include "sdk.h"
#include "singleton.h"
#include "cAutoIdAllocator.h"

class SceneNpc;
class NpcMGroup;
class NpcPGroup;
class GSceneNpcMgr
{
public:
	GSceneNpcMgr();
	~GSceneNpcMgr();
	void final();
public:
	void tick(uint32 group);
	void viewLoop(uint32 group);
	void deadLoop(uint32 group);
	void leaveLoop(uint32 group);
	void fallLoop(uint32 group);
	void bodyLoop(uint32 group);
	void hatchLoop(uint32 group);
	void hiddenLoop(uint32 group);
	void rebornLoop(uint32 group);
	void bornLoop(uint32 group);
	void destroyLoop(uint32 group);
	void aiLoop(uint32 group);
public:
	uint32 allocateUid();
	void freeUid(uint32 uid);
	bool addNpc(SceneNpc* npc);
	void removeNpc(SceneNpc* npc);
	SceneNpc* getNpc(uint32 uid);
	void addToBorn(SceneNpc* npc);
	void removeFromBorn(SceneNpc* npc);
	void addToHatch(SceneNpc* npc);
	void removeFromHatch(SceneNpc* npc);
	void addToDead(SceneNpc* npc);
	void addToDestroy(SceneNpc* npc);
	void addToDestroy(NpcMGroup* npcmgroup);
	void addToDestroy(NpcPGroup* npcpgroup);
	void forceRemoveNpc(SceneNpc* npc);
	void forceRemoveNpcMGroup(NpcMGroup* group);
	void forceRemoveNpcPGroup(NpcPGroup* group);
private:
	cAutoIdAllocator<> _idAllocator;
	std::unordered_map<uint32, SceneNpc*> _npcs;
	uint32 _aiGroup = 0;
	std::vector<std::list<SceneNpc*>> _ainpcs;
	std::list<SceneNpc*> _dead;
	std::map<uint32, std::list<SceneNpc*>> _hatch;
	std::map<uint32, std::list<SceneNpc*>> _leaves;
	std::map<uint32, std::list<SceneNpc*>> _fall;
	std::map<uint32, std::list<SceneNpc*>> _body;
	std::list<SceneNpc*> _hidden;
	std::map<uint32, std::list<SceneNpc*>> _born;
	std::map<uint32, std::list<SceneNpc*>> _reborn;
	std::map<uint32, std::list<NpcMGroup*>> _rebornGroup;
	std::map<uint32, std::list<NpcPGroup*>> _rebornPGroup;
	std::list<SceneNpc*> _destroy;
	std::list<NpcMGroup*> _destroyGroup;
	std::list<NpcPGroup*> _destroyGroup;
};

#define gGSceneNpcMgr Singleton<GSceneNpcMgr>::getInstance()
