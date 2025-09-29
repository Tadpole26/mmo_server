#include "SceneScene.h"
#include "SceneScreen.h"

SceneScene::SceneScene(const uint64 sceneId) :
	cScene(sceneId)
{
	//this->_sceneScreen = new SceneScreen();
}

SceneScene::~SceneScene()
{
}

bool SceneScene::init()
{
	assert(map);
	//getScreen().map = map;
	return true;
}

void SceneScene::final()
{
	//SAFE_DELETE(_sceneScreen);
	cScene::final();
}

void SceneScene::tick()
{
}

bool SceneScene::intoPosition(GameUser* pUser, zPos& inout)
{
	return true;
}

bool SceneScene::rebirthPosition(GameUser* pUser, zPos& inout)
{
	return true;
}

uint32 SceneScene::getRebirthDirect()
{
	return true;
}

void SceneScene::onRoleEnter(GameUser* pUser)
{
	return;
}

void SceneScene::onRoleLeave(GameUser* pUser)
{
	return;
}

void SceneScene::onNpcEnter(SceneNpc* pNpc)
{
	return;
}

void SceneScene::onNpcLeave(SceneNpc* pNpc)
{
	return;
}