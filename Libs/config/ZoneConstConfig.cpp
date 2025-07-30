#include "ZoneConstConfig.h"

namespace config
{
	bool ZoneConstConfig::LoadConfig(const std::string& strContent)
	{
		return LoadCfgFromStr(strContent);
	}

}