#include "CopyConfig.h"

namespace config
{
	bool CopyConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("copy_info", std::bind(&CopyConfig::addCopyInfo, this));
		InitSheet("copy_level_info", std::bind(&CopyConfig::addCopyLevelInfo, this));

		return LoadCfgFromStr(strContent);

	}

	bool CopyConfig::addCopyInfo()
	{
		return true;
	}

	bool CopyConfig::addCopyLevelInfo()
	{
		return true;
	}

	copy_info_t* CopyConfig::getCopyConfig(const uint32 tid)
	{
		auto iter = _copyInfos.find(tid);
		if (iter == _copyInfos.end())
			return nullptr;
		return &(iter->second);
	}

	copy_level_info_t* CopyConfig::getCopyLevelConfig(const uint32 tid)
	{
		auto iter = _copyLevelInfos.find(tid);
		if (iter == _copyLevelInfos.end())
			return nullptr;
		return &(iter->second);
	}
}
