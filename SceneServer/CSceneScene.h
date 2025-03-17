#pragma once

#include "CScene.h"

class CSceneMap;
class CSceneScreen;
class CSceneScene : public CScene
{
public:
	CSceneScene(const uint64 ullSceneId);
	virtual ~CSceneScene();
	virtual bool Init();
	virtual void Final();
public:
	//virtual bool IntoPosition();
	inline CSceneScreen& GetScreen() { return *m_pSceneScreen; }
public:
	CSceneMap* m_pSceneMap = nullptr;
private:
	CSceneScreen* m_pSceneScreen = nullptr;
};
