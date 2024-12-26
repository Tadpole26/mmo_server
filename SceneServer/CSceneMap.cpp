#include "CSceneMap.h"
#include "log_mgr.h"
#include "msg_cfg_ins.pb.h"

#include "CNpcConfig.h"

CSceneMap::CSceneMap(uint32 uiMapId) :
	zMap(uiMapId)
{
}

CSceneMap::~CSceneMap()
{
}

void CSceneMap::Final()
{
}

bool CSceneMap::Init()
{
}

bool CSceneMap::__initMapInfo()
{
	std::string strFilePath = GetMapId() + ".bin";
	std::ifstream file(strFilePath, std::ios::binary | std::ios::in);
	if (!file.is_open())
	{
		Log_Error("init map info, file not exist, %s", strFilePath.c_str());
		return false; 
	}
	packet::Map_Info mapcfg;
	if (!mapcfg.ParseFromIstream(&file))
	{
		Log_Error("init map info, fail to parse, %s", strFilePath.c_str());
		file.close();
		return false;
	}
	SetWidth(mapcfg.width());
	SetHeight(mapcfg.height());
	m_vecInfos.resize(GetWidth() * GetHeight() + 1, 0);
	file.close();
	Log_Info("init map info, ok width:%u, height:%u, path:%s", mapcfg.width(), mapcfg.height(), strFilePath.c_str());
	return true;
}

bool CSceneMap::__initNpc()
{
	std::string strFilePath = std::to_string(GetMapId()) + "_" + std::to_string(packet::BrushType_RigNPC) + ".bin";
	std::ifstream file(strFilePath, std::ios::binary | std::ios::in);
	if (!file.is_open())
	{
		Log_Error("init npc, file not exist, %s", strFilePath.c_str());
		return false;
	}
	packet::Map_AllTypes mapcfg;
	if (!mapcfg.ParseFromIstream(&file))
	{
		Log_Error("init npc, fail to parse, %s", strFilePath.c_str());
		file.close();
		return false;
	}

	bool ret = true;
	for (auto i = 0; i < mapcfg.maprignpc().vecgrids_size(); ++i)
	{
		const packet::Grid_RigNPC& oInfo = mapcfg.maprignpc().vecgrids(i);
		//npc坐标
		if (!oInfo.has_pos())
		{
			Log_Error("init npc, proto data no pos, %s", strFilePath.c_str());
			ret = false;
			continue;
		}
		//npc触发范围
		if (!oInfo.has_triggerrect())
		{
			Log_Error("init npc, proto data no trigger rect, %s", strFilePath.c_str());
			ret = false;
			continue;
		}

		if (!oInfo.has_rota())
		{
			Log_Error("init npc, proto data no rota, %s", strFilePath.c_str());
			ret = false;
			continue;
		}

		const npc_info_t* pNpcCfg = gNpcCfg->GetNpcInfoCfg(oInfo.npcid());
		if (!pNpcCfg)
		{
			Log_Error("init npc, npc tid not in config, %u", oInfo.npcid());
			ret = false;
			continue;
		}
		const auto& oPos = oInfo.pos();
		const auto& oRect = oInfo.triggerrect();
		//表演npc不加载
		if (pNpcCfg->bShow)
		{
			Log_Info("init npc, show npc need not load:%u, [%u,%u], [%u,%u], [%u,%u], %u",
				oInfo.npcid(), oPos.posx(), oPos.posy(), oRect.minpos().posx(), oRect.minpos().posy(), 
				oRect.maxpos().posx(), oRect.maxpos().posy(), oInfo.rota().rotay());
			continue;
		}
		uint32 uiPosX = oPos.posx() / SMALL_POS_OFFSET;
		uint32 uiPosY = oPos.posy() / SMALL_POS_OFFSET;

	}
}

bool CSceneMap::__finalNpc()
{
}

bool CSceneMap::__initTel()
{
	std::string strFilePath = std::to_string(GetMapId()) + "_" + std::to_string(packet::BrushType_Teleporter) +".bin";
	std::ifstream file(strFilePath, std::ios::binary | std::ios::in);
	if (!file.is_open())
	{
		Log_Error("init tel, file not exist, %s", strFilePath.c_str());
		return false;
	}
	packet::Map_AllTypes mapcfg;
	if (!mapcfg.ParseFromIstream(&file))
	{
		Log_Error("init tel, fail to parse, %s", strFilePath.c_str());
		file.close();
		return false;
	}
	//static uint32 uiTelId = 0;
	return true;
}

bool CSceneMap::__checkPosValid(const packet::Grid_Rect& rect)
{
	if (rect.maxpos().posx() / SMALL_POS_OFFSET >= GetWidth() || rect.minpos().posx() / SMALL_POS_OFFSET >= GetWidth())
		return false;

	if (rect.maxpos().posx() / SMALL_POS_OFFSET >= GetHeight() || rect.minpos().posy() / SMALL_POS_OFFSET >= GetHeight())
		return false;
	
	return true;
}