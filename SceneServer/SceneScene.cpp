#include "SceneScene.h"
#include "SceneScreen.h"

SceneScene::SceneScene(const uint64 sceneId) :
	cScene(sceneId)
{
	this->_sceneScreen = new SceneScreen();
}

SceneScene::~SceneScene()
{
}

bool SceneScene::init()
{
	assert(map);
	getScreen().map = map;
	return true;
}

void SceneScene::final()
{
	SAFE_DELETE(_sceneScreen);
	cScene::final();
}