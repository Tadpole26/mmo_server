#pragma once

#include "CCmptView.h"

class CGameUser;
class CCmptUserView : public CCmptView
{
public:
	CCmptUserView(CGameUser* pUser);
	virtual ~CCmptUserView();
	virtual void Final();
	virtual bool Init();
	virtual uint64 GetViewId();
	virtual uint16 GetViewType();
	virtual uint64 GetSceneId();
	virtual uint32 GetMapID();
	virtual uint32 GetSpeed();
private:
	CGameUser* m_pUser = nullptr;
	uint32 m_uiMoveSpeed = 0;
};
