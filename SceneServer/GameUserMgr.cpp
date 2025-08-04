#include "GameUser.h"
#include "GameUserMgr.h"

GameUserMgr::GameUserMgr()
{
}
    
GameUserMgr::~GameUserMgr()
{
}

bool GameUserMgr::init()
{
    return true;
}

void GameUserMgr::final()
{
    for (auto it = _alluser.begin(); it != _alluser.end();)
    {
        auto del = it++;
        auto *pUser = del->second;
        pUser->final();
        SAFE_DELETE(pUser);
    }
}

GameUser* GameUserMgr::createUser(zRoleIdType roleId)
{
    if (!roleId)
    {
        Log_Error("createRole.!roleId");
        return nullptr;
    }
    auto *pUser = new GameUser(roleId);
    if (!pUser)
    {
        Log_Error("createRole,!pRole");
        return nullptr;
    }
    if (!pUser->init())
    {
        Log_Error("createRole.!init");
        return nullptr;
    }
    if (!_alluser.emplace(roleId, pUser).second)
    {
        Log_Error("createRole.!emplace");
        return nullptr;
    }
    return pUser;
}

GameUser* GameUserMgr::getUser(zRoleIdType roleId)
{
    auto it = _alluser.find(roleId);
    if (it == _alluser.end()) return nullptr;
    return it->second;
}