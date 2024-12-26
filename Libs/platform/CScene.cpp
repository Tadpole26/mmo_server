#include "CScene.h"

CScene::CScene(const uint64 ullSceneId) : m_uSid(ullSceneId)
{
}

CScene::~CScene()
{
}

void CScene::Final()
{
}

bool CScene::IsValidMapId(const uint32 uiMapId)
{
	return uiMapId >= MapIdMin && uiMapId <= MapIdMax;
}

bool CScene::IsValidCopyLvId(const uint32 uiCopyLvId)
{
	return uiCopyLvId >= AutoIdValMin && uiCopyLvId <= AutoIdValMax;
}

bool CScene::IsValidAutoId(const uint32 uiAutoId)
{
	return uiAutoId >= CopyLvIdMin && uiAutoId <= CopyLvIdMax;
}

uint32  CScene::SceneId2MapId(const uint64 ullSceneId)
{
	union_scene_id_t temp(ullSceneId);
	return temp.uiMapId;
}

uint32  CScene::SceneId2CopyLvId(const uint64 ullSceneId)
{
	union_scene_id_t temp(ullSceneId);
	return temp.uiCopyLvId;
}

uint32  CScene::SceneId2Param(const uint64 ullSceneId)
{
	union_scene_id_t temp(ullSceneId);
	return temp.eParam;
}

uint32 CScene::SceneId2Eproc(const uint64 ullSceneId)
{
	union_scene_id_t temp(ullSceneId);
	return temp.eProc;
}

uint32  CScene::SceneId2AutoId(const uint64 ullSceneId)
{
	union_scene_id_t temp(ullSceneId);
	return temp.uiAutoId;
}

uint32  CScene::SceneId2CopyType(const uint64 ullSceneId)
{
	//uint32 uiCopyLvId = SceneId2CopyLvId(ullSceneId);
	//if (uiCopyLvId)
	return 0;
}

uint64  CScene::UnionSceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiCopyLvId, uint32 uiAutoId)
{
	union_scene_id_t temp;
	temp.uiMapId = uiMapId;
	temp.eParam = eParam;
	temp.eProc = eProc;
	temp.uiCopyLvId = uiCopyLvId;
	temp.uiAutoId = uiAutoId;
	return temp.ullSceneId;
}

uint64  CScene::UnionFirstSceneId(uint32 eParam, uint32 eProc)
{
	return UnionStaticSceneId(eParam, eProc, FirstMapId, AutoIdValMin);
}

uint64  CScene::UnionStaticSceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiAutoId)
{
	return UnionSceneId(eParam, eProc, uiMapId, 0, uiAutoId);
}

uint64  CScene::UnionCopySceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiCopyLvId, uint32 uiAutoId)
{
	assert(IsValidAutoId(uiAutoId));
	return UnionSceneId(eParam, eProc, uiMapId, uiCopyLvId, uiAutoId);
}