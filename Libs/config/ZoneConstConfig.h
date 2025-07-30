#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	class ZoneConstConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;
	private:
	private:
		UINT32 _zoneId = 0;
		std::string strIp = "";
	};
}
