#include "NpcConfig.h"

namespace config
{
	bool NpcConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("item_info", std::bind(&NpcConfig::AddNpcInfo, this));

		return LoadCfgFromStr(strContent);
	}

	bool NpcConfig::LoadConfigByFile(const std::string& fileName)
	{
		InitSheet("item_info", std::bind(&NpcConfig::AddNpcInfo, this));

		return LoadCfgFromXml(fileName.c_str());
	}

	const npc_info_t* NpcConfig::GetNpcInfoCfg(uint32 uiTid) const
	{
		auto iter = m_mapNpcInfoCfg.find(uiTid);
		if (iter == m_mapNpcInfoCfg.end())
			return nullptr;
		return &(iter->second);
	}

	bool NpcConfig::AddNpcInfo()
	{
		uint32_t uiTid = GetDataUInt("id");
		npc_info_t& stCfg = m_mapNpcInfoCfg[uiTid];
		stCfg.uiTid = uiTid;
		stCfg.strComment = GetDataChr("comment");
		stCfg.uiType = GetDataUInt("type");
		stCfg.bShow = GetDataBool("show");
		stCfg.uiConGroupId = GetDataUInt("con_group");
		stCfg.strFunc = GetDataChr("function");
		stCfg.strFuncCon = GetDataChr("function_con");
		stCfg.strTolkPos = GetDataChr("talk_pos");
		return true;
	}
}




