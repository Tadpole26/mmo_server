#pragma once
#include "sdk.h"
#include "singleton.h"

class GameUser;
class GameUserMgr
{
public:
    GameUserMgr();
    ~GameUserMgr();
    bool init();
    void final();
public:
    GameUser* createUser(zRoleIdType roleId);
    GameUser* getUser(zRoleIdType roleId);
private:
    std::unordered_map<zRoleIdType, GameUser*> _alluser;
};

#define gGameUserMgrIns Singleton<GameUserMgr>::getInstance()