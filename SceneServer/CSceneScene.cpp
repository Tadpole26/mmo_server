#include "CSceneScene.h"
#include "CSceneScreen.h"

CSceneScene::CSceneScene(const uint64 ullSceneId) :
	CScene(ullSceneId)
{
	this->m_pSceneScreen = new CSceneScreen();
}

CSceneScene::~CSceneScene()
{
}

bool CSceneScene::Init()
{
	assert(m_pSceneMap);
	GetScreen().m_pMap = m_pSceneMap;
	return true;
}

void CSceneScene::Final()
{
	SAFE_DELETE(m_pSceneScreen);

	CScene::Final();
}