#pragma once

#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
	struct copy_info_t
	{
		uint32 tid = 0;
		uint32 type = 0;		//��������
	};

	struct copy_level_info_t
	{
		uint32 tid = 0;			//�����ؿ�id
		uint32 copyId = 0;		//��������id
		uint32 layerLevel = 0;	//�ؿ��ȼ�
	};


	class CopyConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;

		copy_info_t* getCopyConfig(const uint32 tid);
		copy_level_info_t* getCopyLevelConfig(const uint32 tid);
	private:
		bool addCopyInfo();
		bool addCopyLevelInfo();
	private:
		std::unordered_map<uint32_t, copy_info_t> _copyInfos;
		std::unordered_map<uint32_t, copy_level_info_t> _copyLevelInfos;
	};
}

#define gCopyCfg Singleton<config::CopyConfig>::getInstance()
