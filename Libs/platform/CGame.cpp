#include "CGame.h"
#include "client.pb.h"

namespace cGame
{
	uint32 uiDBNum						= 8;								
	uint32 uiDBTableNum					= 32;			
	uint32 uiCreateRoleMax				= roleNumIdValMax + 1;

	void initGlobal()
	{
		struct ZoneRoleModuleInfo
		{
			std::string ename;
			uint32 type = 0;
			ZoneRoleModuleInfo(uint32 type, const std::string& ename)
			{
				auto& mt = allTypeIdsRef();
				auto& mn = allENamesRef();
				mt[ename] = type;
				mn[type] = ename;
			}
		};
		std::vector<ZoneRoleModuleInfo> allmodules
		{
			ZoneRoleModuleInfo(client::enModule_Login, "login"),
			ZoneRoleModuleInfo(client::enModule_Hero, "hero"),
			ZoneRoleModuleInfo(client::enModule_Friend, "friend"),
			ZoneRoleModuleInfo(client::enModule_Mall, "mall"),
			ZoneRoleModuleInfo(client::enModule_Mail, "mail"),
			ZoneRoleModuleInfo(client::enModule_Item, "item"),
		};
	}

	std::map<std::string, uint32>& allTypeIdsRef()
	{
		static std::map<std::string, uint32> moduleTypes;
		return moduleTypes;
	}

	std::map<uint32, std::string>& allENamesRef()
	{
		static std::map<uint32, std::string> moduleNames;
		return moduleNames;
	}

	uint32 ename2typeId(const std::string& ename)
	{
		auto it = allTypeIdsRef().find(ename);
		if (it == allTypeIdsRef().end())
		{
			assert(false);
			return 0;
		}
		return it->second;
	}

	const std::string& typeId2ename(uint32 typeId)
	{
		auto it = allENamesRef().find(typeId);
		if (it == allENamesRef().end())
		{
			assert(false);
			static std::string emptystr;
			return emptystr;
		}
		return it->second;
	}
};