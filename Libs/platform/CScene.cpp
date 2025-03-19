#include "cScene.h"
#include "zLogMgr.h"

cScene::cScene(const uint64 sceneId) : _uSid(sceneId)
{
}

cScene::~cScene()
{
}

void cScene::final()
{
}

bool cScene::isValidMapId(const uint32 mapId)
{
	return mapId >= MapIdMin && mapId <= MapIdMax;
}

bool cScene::isValidCopyLvId(const uint32 copyLvId)
{
	return copyLvId >= CopyLvIdMin && copyLvId <= CopyLvIdMax;
}

bool cScene::isValidAutoId(const uint32 autoId)
{
	return autoId >= AutoIdValMin && autoId <= AutoIdValMax;
}

uint32 cScene::sceneId2MapId(const zSceneIdType sceneId)
{
	UnionSceneId temp(sceneId);
	return temp.mapId;
}

uint32  cScene::sceneId2CopyLvId(const zSceneIdType sceneId)
{
	UnionSceneId temp(sceneId);
	return temp.copyLvId;
}

uint32  cScene::sceneId2Param(const zSceneIdType sceneId)
{
	UnionSceneId temp(sceneId);
	return temp.eParam;
}

uint32 cScene::sceneId2Eproc(const zSceneIdType sceneId)
{
	UnionSceneId temp(sceneId);
	return temp.eProc;
}

uint32 cScene::sceneId2AutoId(const zSceneIdType sceneId)
{
	UnionSceneId temp(sceneId);
	return temp.autoId;
}

uint32 cScene::sceneId2CopyType(const zSceneIdType ullSceneId)
{
	//uint32 uiCopyLvId = SceneId2CopyLvId(ullSceneId);
	//if (uiCopyLvId)
	return 0;
}

zSceneIdType cScene::unionSceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiCopyLvId, uint32 uiAutoId)
{
	UnionSceneId temp;
	temp.mapId = uiMapId;
	temp.eParam = eParam;
	temp.eProc = eProc;
	temp.copyLvId = uiCopyLvId;
	temp.autoId = uiAutoId;
	return temp.sceneId;
}

zSceneIdType cScene::unionFirstSceneId(uint32 eParam, uint32 eProc)
{
	return unionStaticSceneId(eParam, eProc, FirstMapId, AutoIdValMin);
}

zSceneIdType cScene::unionStaticSceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiAutoId)
{
	return unionSceneId(eParam, eProc, uiMapId, 0, uiAutoId);
}

zSceneIdType cScene::unionCopySceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiCopyLvId, uint32 uiAutoId)
{
	assert(isValidAutoId(uiAutoId));
	return unionSceneId(eParam, eProc, uiMapId, uiCopyLvId, uiAutoId);
}

const char* cScene::getCopyTypeStr(const uint32 copyType)
{
	switch (copyType)
	{
	}
	return "CopyType_Unknow";
}

const char* cScene::getRunStateStr(const enSceneRunState state)
{
	switch (state)
	{
	case enSceneRunState_Create:			return "Create";	break;
	case enSceneRunState_Register:			return "Register";	break;
	case enSceneRunState_Running:			return "Running";	break;
	case enSceneRunState_Recycle:			return "Recycle";	break;
	case enSceneRunState_Remove:			return "Remove";	break;
	}
	return "Unknow";
}

const std::string cScene::sceneIdStr(const zSceneIdType sceneId)
{
	UnionSceneId temp(sceneId);
	return "aaaaaaaaaaa";
}

void cScene::setRegister()
{
	assert(_runstate == enSceneRunState_Create);
	__setRunState(enSceneRunState_Register);
}

void cScene::setRunning()
{
	assert(_runstate == enSceneRunState_Create || _runstate == enSceneRunState_Register);
	__setRunState(enSceneRunState_Running);
}

void cScene::setRecycle()
{
	assert(_runstate != enSceneRunState_Recycle && _runstate != enSceneRunState_Remove);
	__setRunState(enSceneRunState_Recycle);
}

void cScene::setRemove()
{
	assert(_runstate == enSceneRunState_Recycle);
	__setRunState(enSceneRunState_Remove);
}

void cScene::__setRunState(const enSceneRunState state)
{
	auto old = _runstate;
	_runstate = state;
	Log_Info("__setRunState,%s->%s", getRunStateStr(old), getRunStateStr(_runstate));
}