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
		return iter->second;
	}

	copy_level_info_t* CopyConfig::getCopyLevelConfig(const uint32 tid)
	{
		auto iter = _copyLevelInfos.find(tid);
		if (iter == _copyLevelInfos.end())
			return nullptr;
		return &(iter->second);
	}

	bool CopyConfig::foreachCopyId(uint32 eCopyType, std::function<bool(const copy_info_t&)> f)
	{
		auto iterFind = _typeCopys.find(eCopyType);
		if (iterFind != _typeCopys.end())
		{
			for (const auto* cfg : iterFind->second)
			{
				if (!f(*cfg)) return false;
			}
		}
		return true;
	}
}
