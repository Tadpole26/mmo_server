#include "CSceneMap.h"
#include "CSceneMapMgr.h"
#include "log_mgr.h"
#include "msg_cfg_ins.pb.h"

#include "CNpcConfig.h"
#include "CMapConfig.h"
#include "CMonsterConfig.h"

map_area_t Convert(const packet::Grid_Rect& rect)
{
	map_area_t ret;
	if (rect.minpos().posx() > rect.maxpos().posx())
	{
		ret.oLeftdown.x = rect.maxpos().posx() / SMALL_POS_OFFSET;
		ret.oLeftdown.offsetx = rect.maxpos().posx() % SMALL_POS_OFFSET;
		ret.oRightup.x = rect.minpos().posx() / SMALL_POS_OFFSET;
		ret.oRightup.offsetx = rect.minpos().posx() % SMALL_POS_OFFSET;
	}
	else
	{
		ret.oLeftdown.x = rect.minpos().posx() / SMALL_POS_OFFSET;
		ret.oLeftdown.offsetx = rect.minpos().posx() % SMALL_POS_OFFSET;
		ret.oRightup.x = rect.maxpos().posx() / SMALL_POS_OFFSET;
		ret.oRightup.offsetx = rect.maxpos().posx() % SMALL_POS_OFFSET;
	}

	if (rect.minpos().posy() > rect.maxpos().posy())
	{
		ret.oLeftdown.y = rect.maxpos().posy() / SMALL_POS_OFFSET;
		ret.oLeftdown.offsety = rect.maxpos().posy() % SMALL_POS_OFFSET;
		ret.oRightup.y = rect.minpos().posy() / SMALL_POS_OFFSET;
		ret.oRightup.offsety = rect.minpos().posy() % SMALL_POS_OFFSET;
	}
	else
	{
		ret.oLeftdown.y = rect.minpos().posy() / SMALL_POS_OFFSET;
		ret.oLeftdown.offsety = rect.minpos().posy() % SMALL_POS_OFFSET;
		ret.oRightup.y = rect.maxpos().posy() / SMALL_POS_OFFSET;
		ret.oRightup.offsety = rect.maxpos().posy() % SMALL_POS_OFFSET;
	}
	return ret;
}

CSceneMap::CSceneMap(uint32 uiMapId) :
	zMap(uiMapId)
{
}

CSceneMap::~CSceneMap()
{
}

void CSceneMap::Final()
{
	__FinalNpc();
	__FinalTel();
	__FinalMNpc();
	zMap::Final();
}

bool CSceneMap::Init()
{
	if (!__InitMapInfo())
	{
		Log_Error("加载地图失败,地编加载失败");
		return false;
	}
	const auto& mapBrush = gSceneMapMgr->GetAllBrushFiles();
	auto iterMap = mapBrush.find(GetMapId());
	if (iterMap == mapBrush.end())
	{
		Log_Error("加载笔刷文件失败,笔刷文件不存在");
		return true;
	}
	const auto& typeBrush = iterMap->second;
	for (const auto type : typeBrush)
	{
		switch (type)
		{
		case packet::BrushType_RigNPC: if (!__InitNpc()) return false; break;
		case packet::BrushType_Teleporter: if (!__InitTel()) return false; break;
		case packet::BrushType_MonsterGroup: if (!__InitMNpc()) return false; break;
		default:
			{
				Log_Error("加载地图失败,无法识别的笔刷类型:[%u,%u] %u",GetWidth(), GetHeight(), type);
				return false;
			}
		}
	}
	Log_Info("加载地图成功");
	return true;
}

bool CSceneMap::__InitMapInfo()
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

bool CSceneMap::__InitNpc()
{
	std::string strFilePath = std::to_string(GetMapId()) + "_" + std::to_string(packet::BrushType_RigNPC) + ".bin";
	std::ifstream file(strFilePath, std::ios::binary | std::ios::in);
	if (!file.is_open())
	{
		Log_Error("初始化固定Npc:笔刷文件不存在, %s", strFilePath.c_str());
		return false;
	}
	packet::Map_AllTypes mapcfg;
	if (!mapcfg.ParseFromIstream(&file))
	{
		Log_Error("初始化固定Npc:笔刷文件解析失败, %s", strFilePath.c_str());
		file.close();
		return false;
	}

	bool bRet = true;
	for (auto i = 0; i < mapcfg.maprignpc().vecgrids_size(); ++i)
	{
		const packet::Grid_RigNPC& oInfo = mapcfg.maprignpc().vecgrids(i);
		//npc坐标
		if (!oInfo.has_pos())
		{
			Log_Error("初始化固定Npc:没有坐标, %s", strFilePath.c_str());
			bRet = false;
			continue;
		}
		//npc触发范围
		if (!oInfo.has_triggerrect())
		{
			Log_Error("初始化固定Npc:没有触发范围, %s", strFilePath.c_str());
			bRet = false;
			continue;
		}

		if (!oInfo.has_rota())
		{
			Log_Error("初始化固定Npc:proto data no rota, %s", strFilePath.c_str());
			bRet = false;
			continue;
		}

		const npc_info_t* pNpcCfg = gNpcCfg->GetNpcInfoCfg(oInfo.npcid());
		if (!pNpcCfg)
		{
			Log_Error("初始化固定Npc:配置不存在, %u", oInfo.npcid());
			bRet = false;
			continue;
		}
		const auto& oPos = oInfo.pos();
		const auto& oRect = oInfo.triggerrect();
		//表演npc不加载
		if (pNpcCfg->bShow)
		{
			Log_Info("初始化固定Npc:表演Npc不需要加载:%u, [%u,%u], [%u,%u], [%u,%u], %u",
				oInfo.npcid(), oPos.posx(), oPos.posy(), oRect.minpos().posx(), oRect.minpos().posy(), 
				oRect.maxpos().posx(), oRect.maxpos().posy(), oInfo.rota().rotay());
			continue;
		}
		uint32 uiPosX = oPos.posx() / SMALL_POS_OFFSET;
		uint32 uiPosY = oPos.posy() / SMALL_POS_OFFSET;
		if (!__CheckPosValid(oRect))
		{
			Log_Error("初始化固定Npc:范围坐标不合法, %u, [%u,%u], [%u,%u], [%u,%u], %u",
				oInfo.npcid(), oPos.posx(), oPos.posy(), oRect.minpos().posx(), oRect.minpos().posy(),
				oRect.maxpos().posx(), oRect.maxpos().posy(), oInfo.rota().rotay());
			bRet = false;
			continue;
		}
		if (uiPosX >= GetWidth() || uiPosY >= GetHeight())
		{
			Log_Error("初始化固定Npc:左边不合法, %u, [%u,%u], [%u,%u], [%u,%u], %u",
				oInfo.npcid(), oPos.posx(), oPos.posy(), oRect.minpos().posx(), oRect.minpos().posy(),
				oRect.maxpos().posx(), oRect.maxpos().posy(), oInfo.rota().rotay());
			bRet = false;
			continue;
		}

		auto* area = new map_area_t;
		*area = Convert(oRect);
		auto* pFnpc = new map_fnpc_t();
		pFnpc->uiTid = oInfo.npcid();
		pFnpc->uiCircledir = oInfo.rota().rotay() / SMALL_POS_OFFSET;
		pFnpc->oPos.x = uiPosX;
		pFnpc->oPos.offsetx = oPos.posx() % SMALL_POS_OFFSET;
		pFnpc->oPos.y = uiPosY;
		pFnpc->oPos.offsety = oPos.posy() % SMALL_POS_OFFSET;
		pFnpc->pArea = area;
		m_mapFNpc[oInfo.npcid()] = pFnpc;
		Log_Info("初始化固定Npc:%u, [%u,%u], [%u,%u], [%u,%u], %u",
			oInfo.npcid(), oPos.posx(), oPos.posy(), oRect.minpos().posx(), oRect.minpos().posy(),
			oRect.maxpos().posx(), oRect.maxpos().posy(), oInfo.rota().rotay());
	}
	file.close();
	if (!bRet)
	{
		Log_Error("初始化固定Npc:失败, %s", strFilePath.c_str());
		return false;
	}
	Log_Info("初始化固定Npc:成功, %s",strFilePath.c_str());
	return true;
}

void CSceneMap::__FinalNpc()
{
	for (auto& pair : m_mapFNpc)
	{
		auto* pFnpc = const_cast<map_fnpc_t*>(pair.second);
		SAFE_DELETE(pFnpc->pArea);
		SAFE_DELETE(pFnpc);
	}
}

bool CSceneMap::__InitTel()
{
	std::string strFilePath = std::to_string(GetMapId()) + "_" + std::to_string(packet::BrushType_Teleporter) +".bin";
	std::ifstream file(strFilePath, std::ios::binary | std::ios::in);
	if (!file.is_open())
	{
		Log_Error("初始化传送点:笔刷文件不存在, %s", strFilePath.c_str());
		return false;
	}
	packet::Map_AllTypes mapcfg;
	if (!mapcfg.ParseFromIstream(&file))
	{
		Log_Error("初始化传送点:解析笔刷文件失败, %s", strFilePath.c_str());
		file.close();
		return false;
	}
	static uint32 uiTelId = 0;
	bool bRet = true;
	for (auto i = 0; i < mapcfg.maptel().vecgrids_size(); ++i)
	{
		const packet::Grid_Teleporter& telInfo = mapcfg.maptel().vecgrids(i);
		if (!telInfo.has_pos())
		{
			Log_Error("初始化传送点:坐标不存在, %s", strFilePath.c_str());
			bRet = false;
			continue;
		}
		if (!telInfo.has_triggerrect())
		{
			Log_Error("初始化传送点:区域范围不存在, %s", strFilePath.c_str());
			bRet = false;
			continue;
		}
		const auto& oPos = telInfo.pos();
		const auto& oRect = telInfo.triggerrect();
		if (!__CheckPosValid(oRect))
		{
			Log_Error("初始化传送点:范围不合法, %u, [%u,%u], [%u,%u], [%u,%u], %u",
				telInfo.teltype(), oPos.posx(), oPos.posy(), oRect.minpos().posx(), oRect.minpos().posy(),
				oRect.maxpos().posx(), oRect.maxpos().posy(), telInfo.rota().rotay());
			bRet = false;
			continue;
		}
		auto* pTelArea = new map_tel_t();
		*(map_area_t*)pTelArea = Convert(oRect);
		pTelArea->uiTargetMapTid = telInfo.targetmap();
		pTelArea->oTargetPos.x = oPos.posx() / SMALL_POS_OFFSET;
		pTelArea->oTargetPos.offsetx = oPos.posx() % SMALL_POS_OFFSET;
		pTelArea->oTargetPos.y = oPos.posy() / SMALL_POS_OFFSET;
		pTelArea->oTargetPos.offsety = oPos.posy() % SMALL_POS_OFFSET;
		m_mapTels[++uiTelId] = pTelArea;
		if (!pTelArea->ForeachGrid([&](uint32 ix, uint32 iy)->bool
		{
				__SetGridTel(ix, iy);
				auto* pGrid = __CreateMapGrid(ix, iy);
				if (pGrid)
				{
					if (pGrid->pTel)
						return false;
					pGrid->pTel = pTelArea;
				}
				return true;
		}))
		{
			Log_Error("初始化传送点:触发区域有交叉,%u, [%u,%u], [%u,%u], [%u,%u], %u",
				telInfo.teltype(), oPos.posx(), oPos.posy(), oRect.minpos().posx(), oRect.minpos().posy(),
				oRect.maxpos().posx(), oRect.maxpos().posy(), telInfo.rota().rotay());
			continue;
		}
		Log_Info("初始化传送点: %u, [%u,%u], [%u,%u], [%u,%u], %u",
			telInfo.teltype(), oPos.posx(), oPos.posy(), oRect.minpos().posx(), oRect.minpos().posy(),
			oRect.maxpos().posx(), oRect.maxpos().posy(), telInfo.rota().rotay());
	}
	file.close();
	if (!bRet)
	{
		Log_Error("初始化传送点:失败, %s", strFilePath.c_str());
		return false;
	}
	Log_Info("初始化传送点:成功, %s", strFilePath.c_str());
	return true;
}

void CSceneMap::__FinalTel()
{
	for (auto& pair : m_mapTels)
	{
		auto* pTel = pair.second;
		SAFE_DELETE(pTel);
	}
	m_mapTels.clear();
}

bool CSceneMap::__InitMNpc()
{
	std::string strFilePath = std::to_string(GetMapId()) + "_" + std::to_string(packet::BrushType_MonsterGroup) + ".bin";
	std::ifstream file(strFilePath, std::ios::binary | std::ios::in);
	if (!file.is_open())
	{
		Log_Error("初始化怪物组:笔刷文件不存在, %s", strFilePath.c_str());
		return false;
	}
	packet::Map_AllTypes mapcfg;
	if (!mapcfg.ParseFromIstream(&file))
	{
		Log_Error("初始化怪物组:解析笔刷文件失败, %s", strFilePath.c_str());
		file.close();
		return false;
	}
	bool bRet = true;
	for (auto i = 0; i < mapcfg.mapmonstergroup().vecgrids_size(); ++i)
	{
		const auto& groupInfo = mapcfg.mapmonstergroup().vecgrids(i);
		for (auto j = 0; j < groupInfo.vecgroups_size(); ++j)
		{
			const auto& mnpcInfo = groupInfo.vecgroups(j);
			if (!mnpcInfo.has_pos())
			{
				Log_Error("初始化怪物组:坐标不存在, %s", strFilePath.c_str());
				bRet = false;
				continue;
			}
			if (!mnpcInfo.has_createrect())
			{
				Log_Error("初始化怪物组:区域范围不存在, %s", strFilePath.c_str());
				bRet = false;
				continue;
			}
			const auto* pGroupCfg = gMonsterCfg->GetMonsterGroupConfig(mnpcInfo.monstergroupid());
			if (!pGroupCfg)
			{
				Log_Error("初始化怪物组:配置不存在, %u", mnpcInfo.monstergroupid());
				bRet = false;
				continue;
			}
			const auto& oRect = mnpcInfo.createrect();
			if (!__CheckPosValid(oRect))
			{
				Log_Error("初始化怪物组:范围不合法, %u, [%u,%u], [%u,%u]", mnpcInfo.monstergroupid(), 
					oRect.minpos().posx(), oRect.minpos().posy(),oRect.maxpos().posx(), oRect.maxpos().posy());
				bRet = false;
				continue;
			}
			auto* pArea = new map_area_t;
			*pArea = Convert(oRect);
			auto* pMnpc = new map_mnpc_t();
			pMnpc->uiTid = mnpcInfo.monstergroupid();
			pMnpc->pArea = pArea;
			if (!__FillMNpcGroupPos(*pMnpc, *pGroupCfg))
			{
				Log_Error("初始化怪物组:笔刷坐标检查失败, %u, [%u,%u], [%u,%u]", mnpcInfo.monstergroupid(),
					oRect.minpos().posx(), oRect.minpos().posy(), oRect.maxpos().posx(), oRect.maxpos().posy());
				SAFE_DELETE(pArea);
				SAFE_DELETE(pMnpc);
				bRet = false;
				continue;
			}
			if (pGroupCfg->uiCreateTime)
			{
				m_mapDynamicMNpcGroups[mnpcInfo.monstergroupid()].push_back(pMnpc);
			}
			else
			{
				m_mapStaticMNpcGroups[mnpcInfo.monstergroupid()].push_back(pMnpc);
			}
			Log_Info("初始化怪物组: %u, [%u,%u], [%u,%u], [%u,%u], %u",
				mnpcInfo.monstergroupid(), mnpcInfo.pos().posx(), mnpcInfo.pos().posy(), oRect.minpos().posx(), oRect.minpos().posy(),
				oRect.maxpos().posx(), oRect.maxpos().posy());
		}
	}
	file.close();
	if (!bRet)
	{
		Log_Error("初始化怪物组:失败, %s", strFilePath.c_str());
		return false;
	}
	Log_Info("初始化怪物组:成功, %s", strFilePath.c_str());
	return true;
}

void CSceneMap::__FinalMNpc()
{
	for (auto& pair : m_mapStaticMNpcGroups)
	{
		auto& groupmgr = pair.second;
		for (auto j = 0; j < groupmgr.size(); ++j)
		{
			auto* ins = const_cast<map_mnpc_t*>(groupmgr[j]);
			SAFE_DELETE(ins->pArea);
			SAFE_DELETE(ins);
		}
	}
	m_mapStaticMNpcGroups.clear();

	for (auto& pair : m_mapDynamicMNpcGroups)
	{
		auto& groupmgr = pair.second;
		for (auto j = 0; j < groupmgr.size(); ++j)
		{
			auto* ins = const_cast<map_mnpc_t*>(groupmgr[j]);
			SAFE_DELETE(ins->pArea);
			SAFE_DELETE(ins);
		}
	}
	m_mapDynamicMNpcGroups.clear();
}

bool CSceneMap::__CheckPosValid(const packet::Grid_Rect& rect)
{
	if (rect.maxpos().posx() / SMALL_POS_OFFSET >= GetWidth() || rect.minpos().posx() / SMALL_POS_OFFSET >= GetWidth())
		return false;

	if (rect.maxpos().posx() / SMALL_POS_OFFSET >= GetHeight() || rect.minpos().posy() / SMALL_POS_OFFSET >= GetHeight())
		return false;
	
	return true;
}

map_grid_t* CSceneMap::__CreateMapGrid(const uint32 x, const uint32 y)
{
	uint32 uiGridId = Xy2GridId(x,y);
	auto it = m_mapGrids.find(uiGridId);
	if (it != m_mapGrids.end())
		return it->second;
	auto* pGrid = new map_grid_t;
	pGrid->uiGridId = uiGridId;
	m_mapGrids[pGrid->uiGridId] = pGrid;
	return pGrid;
}

map_grid_t* CSceneMap::__GetMapGrid(const uint32 uiGridId)
{
	auto it = m_mapGrids.find(uiGridId);
	assert(it != m_mapGrids.end());
	return it->second;
}

BYTE& CSceneMap::__GridInfo(const uint32 uiGridId)
{
	ValidGridId(uiGridId);
	return m_vecInfos.at(uiGridId);
}

void CSceneMap::__SetGridTel(const uint32 x, const uint32 y)
{
	auto& byte = __GridInfo(Xy2GridId(x, y));
	byte |= (1 << GridType_Tel);
}

bool CSceneMap::__FillMNpcGroupPos(map_mnpc_t& mnpc, const monster_group_info_t& config)
{
	for (auto i = 0; i < config.vecMonsterIds.size(); ++i)
	{
		uint32 uiMonsterId = config.vecMonsterIds[i];
		const monster_param_t* pCfg = gMonsterCfg->GetMonsterParamConfig(uiMonsterId);
		if (!pCfg)
		{
			assert(false);
			return false;
		}
		const monster_info_t* pMCfg = gMonsterCfg->GetMonsterInfoConfig(pCfg->uiMonsterId);
		if (!pMCfg)
		{
			assert(false);
			return false;
		}
		std::vector<zPos>& vecPos = mnpc.mapPos[uiMonsterId];
		mnpc.pArea->ForeachGrid([&](uint32 x, uint32 y, uint32 dx, uint32 dy)->bool
			{
				zPos pos(x, y, dx, dy);
				if (pCfg->iLocationOffsetX >= 0)
					pos.x = pos.x + (uint32)pCfg->iLocationOffsetX > GetWidth() ? GetWidth() : pos.x + (uint32)pCfg->iLocationOffsetX;
				else
					pos.x = pos.x > (uint32)(-pCfg->iLocationOffsetX) ? pos.x - (uint32)(-pCfg->iLocationOffsetX) : 0;

				if (pCfg->iLocationOffsetY >= 0)
					pos.y = pos.y + (uint32)pCfg->iLocationOffsetY > GetHeight() ? GetHeight() : pos.y + (uint32)pCfg->iLocationOffsetY;
				else
					pos.y = pos.y > (uint32)(-pCfg->iLocationOffsetY) ? pos.y - (uint32)(-pCfg->iLocationOffsetY) : 0;
				vecPos.emplace_back(pos);
				return true;
			});
		if (vecPos.empty())
		{
			Log_Error("没有可用的格子:%u, %u, [%u,%u,%u,%u], [%u,%u,%u,%u]",
				uiMonsterId, mnpc.uiTid, mnpc.pArea->oLeftdown.x, mnpc.pArea->oLeftdown.offsetx, mnpc.pArea->oLeftdown.y, mnpc.pArea->oLeftdown.offsety,
				mnpc.pArea->oRightup.x, mnpc.pArea->oRightup.offsetx, mnpc.pArea->oRightup.y, mnpc.pArea->oRightup.offsety);
			return false;
		}
	}
	return true;
}

bool CSceneMap::IsFullScreen() const
{
	map_info_t* pMapCfg = gMapCfg->GetMapConfig(GetMapId());
	if (pMapCfg == nullptr)
		return true;
	return pMapCfg->uiTiny;
}

zPosI CSceneMap::Pos2zPosI(const zPos& oPos) const
{
	return IsFullScreen() ? kFullScreenIndex : zPos2zPosI(zMap::oBigWH, oPos);
}

bool map_area_t::ForeachGrid(std::function<bool(uint32 x, uint32 y)> func) const
{
	for (uint32 x = oLeftdown.x; x <= oRightup.x; ++x)
	{
		for (uint32 y = oLeftdown.y; y <= oLeftdown.y; ++y)
		{
			if (!func(x, y))
				return false;
		}
	}
	return true;
}

bool map_area_t::ForeachGrid(std::function<bool(uint32 x, uint32 y, uint32 dx, uint32 dy)> func) const
{
	for (uint32 x = oLeftdown.x; x <= oRightup.x; ++x)
	{
		for (uint32 y = oLeftdown.y; y <= oLeftdown.y; ++y)
		{
			if (!func(x, y, oLeftdown.offsetx, oLeftdown.offsety))
				return false;
		}
	}
	return true;
}
