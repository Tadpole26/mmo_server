#include "SceneMap.h"
#include "SceneMapMgr.h"
#include "msg_cfg_ins.pb.h"
#include "NavMeshContext.h"

#include "NpcConfig.h"
#include "MapConfig.h"
#include "MonsterConfig.h"

map_area_t Convert(const packet::Grid_Rect& rect)
{
	map_area_t ret;
	if (rect.minpos().posx() > rect.maxpos().posx())
	{
		ret.leftdown.x = rect.maxpos().posx() / SMALL_POS_OFFSET;
		ret.leftdown.offsetx = rect.maxpos().posx() % SMALL_POS_OFFSET;
		ret.rightup.x = rect.minpos().posx() / SMALL_POS_OFFSET;
		ret.rightup.offsetx = rect.minpos().posx() % SMALL_POS_OFFSET;
	}
	else
	{
		ret.leftdown.x = rect.minpos().posx() / SMALL_POS_OFFSET;
		ret.leftdown.offsetx = rect.minpos().posx() % SMALL_POS_OFFSET;
		ret.rightup.x = rect.maxpos().posx() / SMALL_POS_OFFSET;
		ret.rightup.offsetx = rect.maxpos().posx() % SMALL_POS_OFFSET;
	}

	if (rect.minpos().posy() > rect.maxpos().posy())
	{
		ret.leftdown.y = rect.maxpos().posy() / SMALL_POS_OFFSET;
		ret.leftdown.offsety = rect.maxpos().posy() % SMALL_POS_OFFSET;
		ret.rightup.y = rect.minpos().posy() / SMALL_POS_OFFSET;
		ret.rightup.offsety = rect.minpos().posy() % SMALL_POS_OFFSET;
	}
	else
	{
		ret.leftdown.y = rect.minpos().posy() / SMALL_POS_OFFSET;
		ret.leftdown.offsety = rect.minpos().posy() % SMALL_POS_OFFSET;
		ret.rightup.y = rect.maxpos().posy() / SMALL_POS_OFFSET;
		ret.rightup.offsety = rect.maxpos().posy() % SMALL_POS_OFFSET;
	}
	return ret;
}

SceneMap::SceneMap(uint32 mapId) :
	zMap(mapId)
{
}

SceneMap::~SceneMap()
{
}

void SceneMap::final()
{
	__finalTel();
	__finalNpc();
	__finalMNpc();
	zMap::final();
}

bool SceneMap::init()
{
	if (!__initMapInfo())
	{
		Log_Error("加载地图失败,地编加载失败");
		return false;
	}
	if (!__initNavMesh())
	{
		Log_Error("加载navmesh失败,地编加载失败");
		return false;
	}
	if (!__initMNpc()) return false;
	const auto& mapBrush = gSceneMapMgr->getAllBrushFiles();
	auto iterMap = mapBrush.find(getMapId());
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
		case packet::BrushType_RigNPC: if (!__initNpc()) return false; break;
		case packet::BrushType_Teleporter: if (!__initTel()) return false; break;
		case packet::BrushType_MonsterGroup: if (!__initMNpc()) return false; break;
		default:
			{
				Log_Error("加载地图失败,无法识别的笔刷类型:[%u,%u] %u",getWidth(), getHeight(), type);
				return false;
			}
		}
	}
	Log_Info("加载地图成功");
	return true;
}

bool SceneMap::__initMapInfo()
{
	/*
	std::string strFilePath = getMapId() + ".bin";
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
	setWidth(mapcfg.width());
	setHeight(mapcfg.height());
	_infos.resize(getWidth() * getHeight() + 1, 0);
	file.close();
	*/
	packet::Map_Info mapcfg;
	mapcfg.set_width(1024);
	mapcfg.set_height(1024);
	setWidth(mapcfg.width());
	setHeight(mapcfg.height());
	_infos.resize(getWidth() * getHeight() + 1, 0);
	Log_Info("init map info, ok width:%u, height:%u", mapcfg.width(), mapcfg.height());
	return true;
}

bool SceneMap::__initNpc()
{
	std::string strFilePath = std::to_string(getMapId()) + "_" + std::to_string(packet::BrushType_RigNPC) + ".bin";
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

		const config::npc_info_t* pNpcCfg = gNpcCfg->GetNpcInfoCfg(oInfo.npcid());
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
		if (!__checkPosValid(oRect))
		{
			Log_Error("初始化固定Npc:范围坐标不合法, %u, [%u,%u], [%u,%u], [%u,%u], %u",
				oInfo.npcid(), oPos.posx(), oPos.posy(), oRect.minpos().posx(), oRect.minpos().posy(),
				oRect.maxpos().posx(), oRect.maxpos().posy(), oInfo.rota().rotay());
			bRet = false;
			continue;
		}
		if (uiPosX >= getWidth() || uiPosY >= getHeight())
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
		pFnpc->tid = oInfo.npcid();
		pFnpc->circledir = oInfo.rota().rotay() / SMALL_POS_OFFSET;
		pFnpc->pos.x = uiPosX;
		pFnpc->pos.offsetx = oPos.posx() % SMALL_POS_OFFSET;
		pFnpc->pos.y = uiPosY;
		pFnpc->pos.offsety = oPos.posy() % SMALL_POS_OFFSET;
		pFnpc->area = area;
		_fnpcs[oInfo.npcid()] = pFnpc;
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

void SceneMap::__finalNpc()
{
	for (auto& pair : _fnpcs)
	{
		auto* pFnpc = const_cast<map_fnpc_t*>(pair.second);
		SAFE_DELETE(pFnpc->area);
		SAFE_DELETE(pFnpc);
	}
}

bool SceneMap::__initTel()
{
	std::string strFilePath = std::to_string(getMapId()) + "_" + std::to_string(packet::BrushType_Teleporter) +".bin";
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
		if (!__checkPosValid(oRect))
		{
			Log_Error("初始化传送点:范围不合法, %u, [%u,%u], [%u,%u], [%u,%u], %u",
				telInfo.teltype(), oPos.posx(), oPos.posy(), oRect.minpos().posx(), oRect.minpos().posy(),
				oRect.maxpos().posx(), oRect.maxpos().posy(), telInfo.rota().rotay());
			bRet = false;
			continue;
		}
		auto* pTelArea = new map_tel_t();
		*(map_area_t*)pTelArea = Convert(oRect);
		pTelArea->targetMapTid = telInfo.targetmap();
		pTelArea->targetPos.x = oPos.posx() / SMALL_POS_OFFSET;
		pTelArea->targetPos.offsetx = oPos.posx() % SMALL_POS_OFFSET;
		pTelArea->targetPos.y = oPos.posy() / SMALL_POS_OFFSET;
		pTelArea->targetPos.offsety = oPos.posy() % SMALL_POS_OFFSET;
		_tels[++uiTelId] = pTelArea;
		if (!pTelArea->foreachGrid([&](uint32 ix, uint32 iy)->bool
		{
				__setGridTel(ix, iy);
				auto* pGrid = __createMapGrid(ix, iy);
				if (pGrid)
				{
					if (pGrid->tel)
						return false;
					pGrid->tel = pTelArea;
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

void SceneMap::__finalTel()
{
	for (auto& pair : _tels)
	{
		auto* pTel = pair.second;
		SAFE_DELETE(pTel);
	}
	_tels.clear();
}

bool SceneMap::__initMNpc()
{
	/*
	std::string strFilePath = std::to_string(getMapId()) + "_" + std::to_string(packet::BrushType_MonsterGroup) + ".bin";
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
	*/
	packet::Map_AllTypes mapcfg;
	auto& npcMGroup = *mapcfg.mutable_mapmonstergroup();
	auto& npcGride = *npcMGroup.add_vecgrids();
	auto& group = *npcGride.add_vecgroups();
	group.set_monstergroupid(10240);
	group.mutable_pos()->set_posx(8355000);
	group.mutable_pos()->set_posy(795000);
	group.mutable_createrect()->mutable_minpos()->set_posx(8280000);
	group.mutable_createrect()->mutable_minpos()->set_posy(720000);
	group.mutable_createrect()->mutable_maxpos()->set_posx(8430000);
	group.mutable_createrect()->mutable_maxpos()->set_posy(870000);

	bool bRet = true;
	for (auto i = 0; i < mapcfg.mapmonstergroup().vecgrids_size(); ++i)
	{
		const auto& groupInfo = mapcfg.mapmonstergroup().vecgrids(i);
		for (auto j = 0; j < groupInfo.vecgroups_size(); ++j)
		{
			const auto& mnpcInfo = groupInfo.vecgroups(j);
			if (!mnpcInfo.has_pos())
			{
				Log_Error("初始化怪物组:坐标不存在");
				bRet = false;
				continue;
			}
			if (!mnpcInfo.has_createrect())
			{
				Log_Error("初始化怪物组:区域范围不存在");
				bRet = false;
				continue;
			}
			config::monster_group_info_t* pGroupCfg = gMonsterCfg->GetMonsterGroupConfig(mnpcInfo.monstergroupid());
			if (!pGroupCfg)
			{
				Log_Error("初始化怪物组:配置不存在, %u", mnpcInfo.monstergroupid());
				bRet = false;
				continue;
			}
			const auto& oRect = mnpcInfo.createrect();
			if (!__checkPosValid(oRect))
			{
				Log_Error("初始化怪物组:范围不合法, %u, [%u,%u], [%u,%u]", mnpcInfo.monstergroupid(), 
					oRect.minpos().posx(), oRect.minpos().posy(),oRect.maxpos().posx(), oRect.maxpos().posy());
				bRet = false;
				continue;
			}
			auto* pArea = new map_area_t;
			*pArea = Convert(oRect);
			auto* pMnpc = new map_mnpc_t();
			pMnpc->tid = mnpcInfo.monstergroupid();
			pMnpc->area = pArea;
			if (!__fillMNpcGroupPos(*pMnpc, *pGroupCfg))
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
				_dynamicMNpcGroups[mnpcInfo.monstergroupid()].push_back(pMnpc);
			}
			else
			{
				_staticMNpcGroups[mnpcInfo.monstergroupid()].push_back(pMnpc);
			}
			Log_Info("初始化怪物组: %u, [%u,%u], [%u,%u], [%u,%u], %u",
				mnpcInfo.monstergroupid(), mnpcInfo.pos().posx(), mnpcInfo.pos().posy(), oRect.minpos().posx(), oRect.minpos().posy(),
				oRect.maxpos().posx(), oRect.maxpos().posy());
		}
	}
	//file.close();
	if (!bRet)
	{
		Log_Error("初始化怪物组:失败");
		return false;
	}
	Log_Info("初始化怪物组:成功");
	return true;
}

bool SceneMap::__initNavMesh()
{
	auto* pMapCfg = gMapCfg->getMapConfig(getMapId());
	if (!pMapCfg) return false;
	if (_navMeshContext != nullptr) return false;
	_navMeshContext = gSceneMapMgr->getNavMesh(pMapCfg->navmesh);
	if (!_navMeshContext)
	{
		Log_Error("!_navMeshContext.%u", pMapCfg->navmesh);
		return false;
	}
	Log_Debug("__initNavMesh success.%u", pMapCfg->navmesh);
	return true;
}

void SceneMap::__finalMNpc()
{
	for (auto& pair : _staticMNpcGroups)
	{
		auto& groupmgr = pair.second;
		for (auto j = 0; j < groupmgr.size(); ++j)
		{
			auto* ins = const_cast<map_mnpc_t*>(groupmgr[j]);
			SAFE_DELETE(ins->area);
			SAFE_DELETE(ins);
		}
	}
	_staticMNpcGroups.clear();

	for (auto& pair : _dynamicMNpcGroups)
	{
		auto& groupmgr = pair.second;
		for (auto j = 0; j < groupmgr.size(); ++j)
		{
			auto* ins = const_cast<map_mnpc_t*>(groupmgr[j]);
			SAFE_DELETE(ins->area);
			SAFE_DELETE(ins);
		}
	}
	_dynamicMNpcGroups.clear();
}

void SceneMap::__finalNavMesh()
{
}

bool SceneMap::__checkPosValid(const packet::Grid_Rect& rect)
{
	if (rect.maxpos().posx() / SMALL_POS_OFFSET >= getWidth() || rect.minpos().posx() / SMALL_POS_OFFSET >= getWidth())
		return false;

	if (rect.maxpos().posx() / SMALL_POS_OFFSET >= getHeight() || rect.minpos().posy() / SMALL_POS_OFFSET >= getHeight())
		return false;
	
	return true;
}

map_grid_t* SceneMap::__createMapGrid(const uint32 x, const uint32 y)
{
	uint32 uiGridId = xy2GridId(x,y);
	auto it = _grids.find(uiGridId);
	if (it != _grids.end())
		return it->second;
	auto* pGrid = new map_grid_t;
	pGrid->gridId = uiGridId;
	_grids[pGrid->gridId] = pGrid;
	return pGrid;
}

map_grid_t* SceneMap::__getMapGrid(const uint32 uiGridId)
{
	auto it = _grids.find(uiGridId);
	assert(it != _grids.end());
	return it->second;
}

BYTE& SceneMap::__gridInfo(const uint32 uiGridId)
{
	validGridId(uiGridId);
	return _infos.at(uiGridId);
}

void SceneMap::__setGridTel(const uint32 x, const uint32 y)
{
	auto& byte = __gridInfo(xy2GridId(x, y));
	byte |= (1 << GridType_Tel);
}

bool SceneMap::__fillMNpcGroupPos(map_mnpc_t& mnpc, const config::monster_group_info_t& config)
{
	for (auto i = 0; i < config.vecMonsterIds.size(); ++i)
	{
		uint32 uiMonsterId = config.vecMonsterIds[i];
		config::monster_param_t* pCfg = gMonsterCfg->GetMonsterParamConfig(uiMonsterId);
		if (!pCfg)
		{
			assert(false);
			return false;
		}
		config::monster_info_t* pMCfg = gMonsterCfg->GetMonsterInfoConfig(pCfg->uiMonsterId);
		if (!pMCfg)
		{
			assert(false);
			return false;
		}
		std::vector<zPos>& vecPos = mnpc.posList[uiMonsterId];
		mnpc.area->foreachGrid([&](uint32 x, uint32 y, uint32 dx, uint32 dy)->bool
			{
				zPos pos(x, y, dx, dy);
				if (pCfg->iLocationOffsetX >= 0)
					pos.x = pos.x + (uint32)pCfg->iLocationOffsetX > getWidth() ? getWidth() : pos.x + (uint32)pCfg->iLocationOffsetX;
				else
					pos.x = pos.x > (uint32)(-pCfg->iLocationOffsetX) ? pos.x - (uint32)(-pCfg->iLocationOffsetX) : 0;

				if (pCfg->iLocationOffsetY >= 0)
					pos.y = pos.y + (uint32)pCfg->iLocationOffsetY > getHeight() ? getHeight() : pos.y + (uint32)pCfg->iLocationOffsetY;
				else
					pos.y = pos.y > (uint32)(-pCfg->iLocationOffsetY) ? pos.y - (uint32)(-pCfg->iLocationOffsetY) : 0;
				vecPos.emplace_back(pos);
				return true;
			});
		if (vecPos.empty())
		{
			Log_Error("没有可用的格子:%u, %u, [%u,%u,%u,%u], [%u,%u,%u,%u]",
				uiMonsterId, mnpc.tid, mnpc.area->leftdown.x, mnpc.area->leftdown.offsetx, mnpc.area->leftdown.y, mnpc.area->leftdown.offsety,
				mnpc.area->rightup.x, mnpc.area->rightup.offsetx, mnpc.area->rightup.y, mnpc.area->rightup.offsety);
			return false;
		}
	}
	return true;
}

const NavMeshContext& SceneMap::getNavMeshContext() const
{ 
	return *_navMeshContext; 
}

bool SceneMap::isFullScreen() const
{
	const config::map_info_t* pMapCfg = gMapCfg->getMapConfig(getMapId());
	if (pMapCfg == nullptr) return true;
	return pMapCfg->uiTiny;
}

zPosI SceneMap::pos2zPosI(const zPos& oPos) const
{
	return isFullScreen() ? kFullScreenIndex : zPos2zPosI(zMap::bigWH, oPos);
}

bool map_area_t::foreachGrid(std::function<bool(uint32 x, uint32 y)> func) const
{
	for (uint32 x = leftdown.x; x <= rightup.x; ++x)
	{
		for (uint32 y = leftdown.y; y <= leftdown.y; ++y)
		{
			if (!func(x, y))
				return false;
		}
	}
	return true;
}

bool map_area_t::foreachGrid(std::function<bool(uint32 x, uint32 y, uint32 dx, uint32 dy)> func) const
{
	for (uint32 x = leftdown.x; x <= rightup.x; ++x)
	{
		for (uint32 y = leftdown.y; y <= leftdown.y; ++y)
		{
			if (!func(x, y, leftdown.offsetx, leftdown.offsety))
				return false;
		}
	}
	return true;
}
