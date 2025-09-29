#pragma once
#include "cLoadBase.h"
#include "singleton.h"

namespace config
{
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


	class NpcConfig : public cLoadBase
	{
	public:
		bool LoadConfig(const std::string& strContent) override;
		bool LoadConfigByFile(const std::string& fileName) override;

		const npc_info_t* GetNpcInfoCfg(uint32 uiTid) const;

	private:
		bool AddNpcInfo();
	private:
		std::map<uint32_t, npc_info_t> m_mapNpcInfoCfg;
	};
}


#define gNpcCfg Singleton<config::NpcConfig>::getInstance()
