#pragma once
#include "util_load_base.h"
#include "singleton.h"

struct npc_info_t
{
	uint32					uiTid = 0;							//唯一id
	std::string				strComment;							//npc描述
	uint32					uiType = 0;							//npc类型
	bool					bShow = true;						//是否是表演npc
	uint32					uiConGroupId = 0;					//对应条件id
	std::string				strFunc;							//功能列表
	std::string				strFuncCon;							//功能组条件
	std::string				strTolkPos;							//npc交互时站位
};


class CNpcConfig : public CUtilLoadBase
{
public:
	bool LoadConfig(const std::string& strContent) override;

	const npc_info_t* GetNpcInfoCfg(uint32 uiTid) const;

private:
	bool AddNpcInfo();
private:
	std::map<uint32_t, npc_info_t> m_mapNpcInfoCfg;
};

#define gNpcCfg Singleton<CNpcConfig>::getInstance()
