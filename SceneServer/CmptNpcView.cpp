#include "SceneScene.h"
#include "SceneMap.h"
#include "SceneNpc.h"
#include "CmptNpcView.h"

CmptNpcView::CmptNpcView(SceneNpc& npc) :
	CmptView(),
	_npc(npc)
{
}

CmptNpcView::~CmptNpcView()
{
}

bool CmptNpcView::init()
{
	return true;
}

void CmptNpcView::final()
{
	CmptView::final();
}

uint64 CmptNpcView::getViewId()
{
	return npcRef().getUid();
}

WORD CmptNpcView::getViewType()
{
	return npcRef().getNpcType();
}

uint64 CmptNpcView::getSceneId()
{
	return npcRef().scene->getSceneId();
}

uint32 CmptNpcView::getMapId()
{
	return npcRef().scene->map->getMapId();
}

SceneScene* CmptNpcView::getScene()
{
	return npcRef().scene;
}

SceneMap* CmptNpcView::getMap()
{
	return npcRef().scene->map;
}