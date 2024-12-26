#include "CNpcConfig.h"


bool CNpcConfig::LoadConfig(const std::string& strContent)
{
	InitSheet("item_info", std::bind(&CNpcConfig::AddNpcInfo, this));

	return LoadCfgFromStr(strContent);
}

const npc_info_t* CNpcConfig::GetNpcInfoCfg(uint32 uiTid) const
{
	auto iter = m_mapNpcInfoCfg.find(uiTid);
	if (iter == m_mapNpcInfoCfg.end())
		return nullptr;
	return &(iter->second);
}

bool CNpcConfig::AddNpcInfo()
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



