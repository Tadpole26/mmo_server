#pragma once
#include "util_load_base.h"
#include "singleton.h"

struct npc_info_t
{
	uint32					uiTid = 0;							//Ψһid
	std::string				strComment;							//npc����
	uint32					uiType = 0;							//npc����
	bool					bShow = true;						//�Ƿ��Ǳ���npc
	uint32					uiConGroupId = 0;					//��Ӧ����id
	std::string				strFunc;							//�����б�
	std::string				strFuncCon;							//����������
	std::string				strTolkPos;							//npc����ʱվλ
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
