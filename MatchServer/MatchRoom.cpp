#include "matchcommon.pb.h"
#include "innercommon.pb.h"
#include "innermatch.pb.h"
//#include "matchsvr.pb.h"

#include "MatchConfig.h"

#include "MatchMem.h"
#include "MatchGroup.h"
#include "MatchRoom.h"
#include "MatchQueueMgr.h"

MatchRoom::MatchRoom(zRoomIdType roomId, std::string&& formStr, MatchQueueMgr& mgr):
	_roomId(roomId),
	_formStr(std::move(formStr)),
	_mgr(mgr)
{
	_createTime = GetCurrTime();
	_groups.resize(cMatch::eRoomGroupIdMax - 1);
}

MatchRoom::~MatchRoom()
{
}

void MatchRoom::final()
{
	if (_copyscene != nullptr)
	{
		SAFE_DELETE(_copyscene);
	}
	if (_createby != nullptr)
	{
		SAFE_DELETE(_createby);
	}
}

void MatchRoom::tick()
{
	if (isTimeForRoomTimeout())
	{
		__setStage(eStageTimeoutRoom);
	}
	(this->*_funcs[_stage])();
}

void MatchRoom::addGroup(MatchGroup& group)
{
	group.foreach([&](auto* pUnit)
	{
		pUnit->removeFromAllQueue();
		switch (group.getGroupId())
		{
		case cMatch::eRoomGroupIdRed: __addRed(pUnit); break;
		case cMatch::eRoomGroupIdNone: __addBlue(pUnit); break;
		default: assert(false); break;
		}
	});
}

void MatchRoom::__addRed(MatchUnit* pUnit)
{
	assert(pUnit != nullptr);
	assert(pUnit->sizeQueue() == 0);
	assert(pUnit->room == nullptr);

	_groups[cMatch::eRoomGroupIdRed - 1].push_back(pUnit);
	pUnit->room = this;
	_initsize += pUnit->sizeMember();
	_cursize += pUnit->sizeMember();
	Log_Info("__addRed");
}

void MatchRoom::__addBlue(MatchUnit* pUnit)
{
	assert(pUnit != nullptr);
	assert(pUnit->sizeQueue() == 0);
	assert(pUnit->room == nullptr);

	_groups[cMatch::ePunishEscapre - 1].push_back(pUnit);
	pUnit->room = this;
	_initsize += pUnit->sizeMember();
	_cursize += pUnit->sizeMember();
	Log_Info("__addBlue");
}

void MatchRoom::removeUnit(MatchUnit* pUnit)
{
	assert(pUnit->room == this);
	cMatch::foreachRoomGroup([&](uint32 groupId)->bool
	{
		auto& gunit = _groups[groupId - 1];
		auto iter = std::find(gunit.begin(), gunit.end(), pUnit);
		if (iter != gunit.end())
		{
			gunit.erase(iter);
			pUnit->room = nullptr;
			_cursize -= pUnit->sizeMember();
			Log_Info("removeUnit");
		}
		return true;
	});
}

uint32 MatchRoom::getConfirmTimeoutExpire()
{
	auto* pMatch = gMatchCfg->getMatchConfig(mgrRef().getMtype());
	if (pMatch)
		return _createTime + pMatch->confirmTime;
	else 
		return _createTime + 5;
}

uint32 MatchRoom::getCreateCopyTimeoutExpire()
{
	return getConfirmTimeoutExpire() + 10;
}

uint32 MatchRoom::getRoomTimeoutExpire()
{
	return getConfirmTimeoutExpire() + 60;
}

bool MatchRoom::isTimeForRoomTimeout()
{
	if (getRoomTimeoutExpire() < GetCurrTime())
	{
		Log_Info("isTimeForRoomTimeout");
		return true;
	}
	return false;
}

bool MatchRoom::isTimeForConfirm()
{
	return _stage == eStageWaitConfirm;
}

bool MatchRoom::isTimeForConfirmRobot()
{
	return true;
}

bool MatchRoom::isTimeForConfirmTimeout()
{
	if (getConfirmTimeoutExpire() > GetCurrTime()) return false;
	Log_Info("isTimeForConfirmTimeout");
	return true;
}

bool MatchRoom::isTimeForCreateCopy()
{
	return _stage == eStageWaitCopyScene;
}

bool MatchRoom::isTimeForCreateCopyTimeout()
{
	if (getCreateCopyTimeoutExpire() > GetCurrTime()) return false;
	Log_Info("isTimeForCreateCopyTimeout");
	return true;
}

void MatchRoom::__tryAddUnitInfo(MatchUnit& unit)
{
	if (_unitZoneList.end() == std::find(_unitZoneList.begin(), _unitZoneList.end(), unit.getZoneId()))
	{
		_unitZoneList.push_back(unit.getZoneId());
	}
}

void MatchRoom::__funcCreateRoom()
{
	inner::InnerMatchRoom room;
	fill(room);
	for (auto zoneId : _unitZoneList)
	{
		/*
		NetInterface::get().sendCmdTeamsvrLocal(zoneId, [&](auto& out)
			{
				auto& ntf = *mutable_frommatch_startconfirm();
				*ntf.mutable_room() = room;
				ntf.set_expire(getConfirmTimieoutExpire());
			});
			*/
	}
	__setStage(eStageWaitConfirm);
}

void MatchRoom::__funcWaitConfirm()
{
	trySetConfirmOk();
	if (isTimeForConfirmTimeout())
	{
		//setConfirmTimeout();
	}
	else if (isTimeForConfirmRobot())
	{
		setConfirmRandRobot();
	}
}

void MatchRoom::__funcWaitCopyScene()
{
	trySetCopySceneOk();
	if (isTimeForCreateCopyTimeout())
	{
		setCopySceneTimeout();
	}
}

void MatchRoom::__funcCancelRoom()
{
	cMatch::foreachRoomGroup([&](auto groupId) ->bool
	{
		return foreachUnit(groupId, [&](auto& unit)->bool
		{
			if (unit.needPunish())
			{
				unit.punishMatch();
			}
			else
			{
				unit.restartMatch();
			}
			return true;
		});
	});
	__setStage(eStageFinishRoom);
}

void MatchRoom::__funcFinishRoom()
{
	cMatch::foreachRoomGroup([&](auto groupId)->bool 
	{
		return foreachUnit(groupId, [&](auto& unit)
		{
			unit.finalMe();
			return true;
		});
	});
	//MatchRoomMgr::get().addToRecycle(this);
}

void MatchRoom::__funcTimeoutRoom()
{
	cMatch::foreachRoomGroup([&](auto groupId)->bool 
		{
			return foreachUnit(groupId, [&](auto& unit)
				{
					unit.restartMatch();
					return true;
				});
		});
	//MatchRoomMgr::get().addToRecycle(this);
}

void MatchRoom::__setStage(uint32 stage)
{
	assert(stage <eStageMax);
	auto old = _stage;
	_stage = stage;
	Log_Info("__setStage");
}

bool MatchRoom::isStageClientConfirm()
{
	return _stage <= eStageWaitConfirm;
}

void  MatchRoom::fill(inner::InnerMatchRoom& out)
{
	out.set_roomid(getRoomId());
	out.set_mtype(mgrRef().getMtype());
	cMatch::foreachRoomGroup([&](auto groupId)->bool 
		{
			auto& one = (*out.mutable_groups())[groupId];
			return foreachUnit(groupId, [&](auto& unit) 
				{
					unit.fill((*one.mutable_units())[unit.getUnitId()]);
					return true;
				});
		});
}

void  MatchRoom::fill(inner::InnerCopyCrit& out)
{
	//out.set_hashidf(MatchServer::get().getHashId());
	//out.set_critid(getRoomId());
	//out.set_expire(getCreateCopyTimeoutExpire());
}

void  MatchRoom::fill(inner::InnerCopyWayByMatch& out)
{
	//out.set_mtype(mgrRef().getMtype());
	//out.set_seasonid(mgrRef().timeRef().getSeasonId(mgrRef().timeRef().getProcThree()));
	//out.set_dayid(mgrRef().timeRef().getDayId());
	//fill(*out.mutable_room());
}

bool MatchRoom::foreachUnit(uint32 groupId, FuncGroupUnitT func)
{
	assert(cMatch::validRoomGroupId(groupId));
	auto& gunit = _groups[groupId - 1];
	for (auto iter = gunit.begin(); iter != gunit.end();)
	{
		auto del = iter++;
		auto* pUnit = *del;
		if (!func(*pUnit)) return false;
	}
	return true;
}

void MatchRoom::forceBreakNtf()
{
	Log_Info("forceBreakNtf");
	__setStage(eStageCancelRoom);
}

void MatchRoom::forceBreak()
{
	Log_Info("forceBreak");
}

void MatchRoom::setConfirmRandRobot()
{
	cMatch::foreachRoomGroup([&](auto groupId) ->bool
	{
		return foreachUnit(groupId, [&](auto& unit)
		{
			return unit.foreachRobot([&](auto& member)
			{
				if (member.isConfirmed()) return true;
				member.setConfirmOk();
				return false;
			});
		});
	});
}

void MatchRoom::setConfirmRefuse()
{
	Log_Info("setConfirmRefuse");
	__setStage(eStageCancelRoom);
}

void MatchRoom::setConfirmTimeout()
{
	Log_Info("setConfirmTimeout");
	cMatch::foreachRoomGroup([&](auto groupId)->bool
	{
		return foreachUnit(groupId, [&](auto& unit)
		{
			return unit.foreach([&](auto& member)
			{
				if (member.isConfirmed()) return true;
				member.setConfirmTimeout();
				return true;
			});
		});
	});
	__setStage(eStageCancelRoom);
}

void MatchRoom::trySetConfirmOk()
{
	if (cMatch::foreachRoomGroup([&](auto groupId)->bool
		{
			return foreachUnit(groupId, [&](auto& unit)
				{
					return unit.foreach([&](auto& member)
						{
							return member.isConfirmedOk();
						});
				});
		}))
	{
		_createby = new inner::InnerCopyCreateBy;
		fill(*_createby->mutable_crit());
		fill(*_createby->mutable_copy());
		auto& way = *_createby->mutable_way();
		fill(*way.mutable_teamlist());
		fill(*way.mutable_bymatch());
		/*
		NetInterface::get().sendCmdMapsvr([&](auto& out)
			{
				auto& ntf = *out.mutable_frommatch_createcopy();
				*ntf.mutable_createby() = *createby;
			});
			*/
		__setStage(eStageWaitCopyScene);
	}
}

void MatchRoom::setCreateCopyResult(zSceneIdType sceneId, uint32 hashId, const inner::InnerCopyCreateBy& createby)
{
	if (isTimeForCreateCopy())
	{
		if (!isTimeForCreateCopyTimeout())
		{
			if (sceneId != 0 && hashId != 0)
			{
				_copyscene = new inner::InnerMatchRoomCopyScene;
				_copyscene->set_expire(0);
				_copyscene->set_sceneid(sceneId);
				_copyscene->set_hashid(hashId);
				Log_Info("setCreateCopyResult.succ");
				return;
			}
			else
				Log_Info("setCreateCopyResult.fail");
		}
		else
			Log_Info("setCreateCopyResult.isTimeForCreateCopyTimeout");
	}
	else 
		Log_Info("setCreateCopyResult.!isTimeForCreateCopy");
	__setStage(eStageCancelRoom);
}

void MatchRoom::trySetCopySceneOk()
{
	if (_copyscene != nullptr)
	{
		for (auto zoneId : _unitZoneList)
		{
			/*
			NetInterface::get().sendCmdTeamsvrLocal(zoneId, [&](auto& out)
				{
					auto& ntf = *out.mutable_frommatch_startloading();
					ntf.set_roomid(getRoomId());
					*ntf.mutable_copyscene() = *_copyscene;
					*ntf.mutable_createby() = *_createby;
				});
				*/
		}
		__setStage(eStageFinishRoom);
	}
}

void MatchRoom::setCopySceneTimeout()
{
	Log_Info("setCopySceneTimeout");
	__setStage(eStageCancelRoom);
}