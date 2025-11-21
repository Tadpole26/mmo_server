#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct copy_info_t
	{
		uint32 tid = 0;
		uint32 type = 0;		//副本类型
	};

	struct copy_level_info_t
	{
		uint32 tid = 0;			//副本关卡id
		uint32 copyId = 0;		//所属副本id
		uint32 layerLevel = 0;	//关卡等级
		uint32 teamId = 0;		//组队id
	};

	class CopyConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;
		bool LoadConfigByFile(const std::string& fileName) override;

		copy_info_t* getCopyConfig(const uint32 tid);
		copy_level_info_t* getCopyLevelConfig(const uint32 tid);
		copy_level_info_t* getFirstLevelConfig(const uint32 copyTid);
		bool foreachCopyId(uint32 eCopyType, std::function<bool(const copy_info_t&)> f);
	private:
		bool addCopyInfo();
		bool addCopyLevelInfo();
	private:
		std::unordered_map<uint32_t, copy_info_t*> _copyInfos;
		std::unordered_map<uint32, std::vector<copy_info_t*>> _typeCopys;
		std::unordered_map<uint32_t, copy_level_info_t*> _copyLevelInfos;
		std::unordered_map<uint32_t, std::vector<copy_level_info_t*>> _typeLevelInfos;
	};
}

#define gCopyCfg Singleton<config::CopyConfig>::getInstance()
