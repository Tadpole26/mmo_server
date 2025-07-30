#include "SceneNpc.h"
#include "CmptNpcView.h"
#include "SceneScene.h"
#include "GSceneNpcMgr.h"

SceneNpc::SceneNpc(uint32 npcuid, SceneNpcMgr& owner, CmptNpcView* viewcmpt) :
	_uid(npcuid),
	_owner(owner),
	_viewcmpt(viewcmpt)
{
}

SceneNpc::~SceneNpc()
{
}

bool SceneNpc::init()
{
	assert(_viewcmpt);
	if (!_viewcmpt->init()) return false;
	return true;
}

void SceneNpc::final()
{
	_viewcmpt->final();
	SAFE_DELETE(_viewcmpt);
	gGSceneNpcMgr->freeUid(getUid());
}

void SceneNpc::bornInitially()
{
	if (_state != NpcState_None)
	{
		assert(false);
		return;
	}
	_stateTimeBegin = GetCurrTime();
	_state = NpcState_Alive;
	_stateTimeEnd = 0;
	viewCmptRef().setStateId(CmptView::enStateId_Normal);
	onAppear();
	scene->onNpcEnter(this);
}

void SceneNpc::bornDynamically()
{
	if (_state != NpcState_None && _state != NpcState_Reborn && _state != NpcState_Delegate)
	{
		assert(false);
		return;
	}
	_state = NpcState_Born;
	_stateTimeBegin = GetCurrTime();
	_stateTimeEnd = _stateTimeBegin + bornInterval();
	viewCmptRef().setStateId(CmptView::enStateId_Born);
	gGSceneNpcMgr->addToBorn(this);
	onAppear();
	scene->onNpcEnter(this);
	//viewCmptRef().sendMeToView()  将信息发送到gatesvr,gatesvr遍历几屏发送
}