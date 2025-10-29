#pragma once

#include "sdk.h"
#include <hiredis.h>

namespace record
{
	class RecordModuleAll;
}

class cRedisMgr
{
public:
	cRedisMgr();
	~cRedisMgr();
public:
	//区服 key:roleId value:模块数据
	const std::string keyTableZoneRole(const zRoleIdType roleId) { return ("TableZoneRole:" + std::to_string(roleId)); }
	bool opTableZoneRoleReadAll(const zRoleIdType roleId, record::RecordModuleAll& out);
	bool opTableZoneRoleSetAll(const zRoleIdType roleId, const record::RecordModuleAll& in);
private:
	bool __hmset(const std::string& key, const std::vector<std::pair<std::string, std::string>>& modules);
	std::pair<bool, std::map<std::string, std::string>> __hgetall(const std::string& key);
private:
	redisContext* c = nullptr;
};