#include "RedisMgr.h"

#include "record.pb.h"

#include "cGame.h"

cRedisMgr::cRedisMgr()
{
	c = redisConnect("127.0.0.1", 6379);
	assert(c != nullptr);
}

cRedisMgr::~cRedisMgr()
{

}

bool cRedisMgr::opTableZoneRoleReadAll(const zRoleIdType roleId, record::RecordModuleAll& out)
{
	if (!c)
	{
		Log_Error("redis conn nullptr");
		return false;
	}
    auto p = __hgetall(keyTableZoneRole(roleId));
    if (!p.first)
    {
        Log_Error("hgetall error, key:%s", keyTableZoneRole(roleId).c_str());
        return false;
    }
    auto& allmodules = *out.mutable_modules();
    for (auto& pair : p.second)
    {
        auto type = cGame::ename2typeId(pair.first);
        if (!allmodules[type].ParseFromString(pair.second))
        {
            out.Clear();
            Log_Error("ParseFromString data error.%u,%s", type, pair.second.c_str());
            return false;
        }
    }
    Log_Info("opTableZoneRoleReadAll success.%lu,%s", roleId, out.ShortDebugString().c_str());
    return true;
}

bool cRedisMgr::opTableZoneRoleSetAll(const zRoleIdType roleId, const record::RecordModuleAll& in)
{
	if (!c)
	{
		Log_Error("redis conn nullptr");
		return false;
	}
	std::vector<std::pair<std::string, std::string>> modules;
	for (const auto& pair : in.modules())
	{
		modules.emplace_back(cGame::typeId2ename(pair.first), pair.second.SerializeAsString());
	}
    if (!__hmset(keyTableZoneRole(roleId), modules))
    {
        Log_Error("opTableZoneRoleSetAll.Fail!");
        return false;
    }
    Log_Info("opTableZoneRoleSetAll success.%lu,%s", roleId, in.ShortDebugString().c_str());
    return true;
}

bool cRedisMgr::__hmset(const std::string& key, const std::vector<std::pair<std::string, std::string>>& modules)
{
    std::vector<const char*> argv;
    std::vector<size_t> argvlen;

    argv.push_back("HMSET");
    argvlen.push_back(5);

    argv.push_back(key.c_str());
    argvlen.push_back(key.length());

    // 添加字段和值对
    for (size_t i = 0; i < modules.size(); ++i) {
        argv.push_back(modules[i].first.c_str());
        argvlen.push_back(modules[i].first.length());

        argv.push_back(modules[i].second.c_str());
        argvlen.push_back(modules[i].second.length());
    }

    // 执行HMSET命令
    redisReply* reply = (redisReply*)redisCommandArgv(c,
        argv.size(),
        &argv[0],
        &argvlen[0]);

    if (reply == nullptr) {
        Log_Error("hmset command error");
        return false;
    }
    return true;
}

std::pair<bool, std::map<std::string, std::string>> cRedisMgr::__hgetall(const std::string& key)
{
    std::pair<bool, std::map<std::string, std::string>> ret;
    ret.first = false;
    // 执行HGETALL命令
    redisReply* reply = (redisReply*)redisCommand(c, key.c_str());
    if (reply == nullptr) {
        Log_Error("hgetall command error");
        return ret;
    }
    // 检查回复类型
    if (reply->type == REDIS_REPLY_ARRAY) {
        ret.first = true;
        for (size_t i = 0; i < reply->elements; i += 2) 
        {
            auto &mapRet = ret.second;
            std::string field = reply->element[i]->str;
            std::string value = reply->element[i + 1]->str;
            mapRet[field] = value;
        }
    }
    else 
    {
        Log_Error("hgetall fail, reply type:%u", reply->type);
        return ret;
    }
    return ret;
}