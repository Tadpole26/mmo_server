#pragma once

#include "CScene.h"

class CSceneMap;

class CSceneScene : public CScene
{
public:
	CSceneScene(const uint64 ullSceneId);
	virtual ~CSceneScene();
	virtual bool Init();
	virtual void Final();
public:
	//virtual bool IntoPosition();
public:
	CSceneMap* m_pSceneMap = nullptr;
};
