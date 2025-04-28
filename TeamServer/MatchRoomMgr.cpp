#include "match.pb.h"
#include "matchcommon.pb.h"
#include "innercommon.pb.h"
#include "innermatch.pb.h"

#include "TeamUser.h"
#include "TeamUserMgr.h"
#include "MatchRoomMgr.h"
#include "MatchUnitMgr.h"

MatchRoom::MatchRoom(const inner::InnerMatchRoom& in)
{
	_room = new inner::InnerMatchRoom(in);
}

MatchRoom::~MatchRoom()
{
}

void MatchRoom::final()
{
	_room->Clear();
	SAFE_DELETE(_room);
}

void MatchRoom::evMatchStartConfirm()
{
}

void MatchRoom::evMatchStartLoading()
{
}

void MatchRoom::fill(common::MatchGroupList& out)
{
	out.set_expire(_expire);
	out.set_stageid(common::enStageId_Confirm);
	for (auto& p : _room->groups())
	{
		switch (p.first)
		{
		case cMatch::eRoomGroupIdRed: *out.mutable_red() = group2group(p.second); break;
		case cMatch::eRoomGroupIdBlue: *out.mutable_blue() = group2group(p.second); break;
		}
	}
}

common::MatchGroup MatchRoom::group2group(const inner::InnerMatchGroup& in)
{
	common::MatchGroup mg;
	for (auto& p : in.units())
	{
		for (auto& p : p.second.robots())
		{
			const auto& one = p.second;
			auto& robot = *mg.add_fighters();
			*robot.mutable_brief() = one.brief();
			*robot.mutable_extd() = one.extd();
			robot.set_isok(isConfirmed(p.first));
		}
		for (auto& p : p.second.roles())
		{
			const auto& one = p.second;
			auto& role = *mg.add_fighters();
			*role.mutable_brief() = one.brief();
			*role.mutable_extd() = one.extd();
			role.set_isok(isConfirmed(p.first));
		}
	}
	return mg;
}

uint32 MatchRoom::getMatchType()
{
	return _room->mtype();
}

zRoomIdType MatchRoom::getRoomId()
{
	return _room->roomid();
}

bool MatchRoom::isExpire()
{
	return _expire < GetCurrTime();
}

bool MatchRoom::isConfirmed(zRoleIdType roleId)
{
	assert(roleId != 0);
	return _confirms.end() != std::find(_confirms.begin(), _confirms.end(), roleId);
}

bool MatchRoom::foreach(std::function<bool(MatchUnit& unit)> func)
{
	for (auto& pUnit : _allunit)
	{
		if (!func(*pUnit)) return false;
	}
	return true;
}

bool MatchRoom::foreach(uint32 groupId, std::function<bool(MatchUnit& unit)> func)
{
	for (auto* pUnit : _allunit)
	{
		if (pUnit->getGroupId() != groupId) continue;
		if (!func(*pUnit)) return false;
	}
	return true;
}

MatchUnit* MatchRoom::getUnit(zUnitIdType unitId)
{
	MatchUnit* pUnit = nullptr;
	foreach([&](auto& unit)->bool
	{
			if (unit.getUnitId() != unitId) return true;
			pUnit = &unit;
			return false;
	});
	return pUnit;
}

void MatchRoom::addUnit(MatchUnit* pUnit)
{
	assert(pUnit != nullptr);
	assert(pUnit->room == nullptr);
	_allunit.push_back(pUnit);
	pUnit->room = this;
	Log_Info("addUnit.%lu",pUnit->getUnitId());
}

void MatchRoom::removeUnit(MatchUnit* pUnit)
{
	assert(pUnit != nullptr);
	assert(pUnit->room == this);
	_allunit.remove(pUnit);
	pUnit->room = nullptr;
	Log_Info("removeUnit.%lu", pUnit->getUnitId());
}

void MatchRoom::startConfirm(uint32 expire)
{
	Log_Info("startConfirm.%u", expire);
	_expire = expire;
	evMatchStartConfirm();
	foreach([&](auto& unit)->bool 
	{
			unit.startConfirm();
			return true;
	});
}

void MatchRoom::setConfirmOk(zRoleIdType roleId)
{
	Log_Info("setConfirmOk.%lu", roleId);
	assert(roleId != 0);
	_confirms.push_back(roleId);
	client::ModuleMatch_Ntf_ConfirmOk ntf;
	ntf.set_roleid(roleId);
	foreach([&](auto& unit)->bool
	{
			return unit.foreach([&](auto& member)->bool
			{
					//return member.sendCmdToMe(client::enClientFirst_Match, client::enSecondMatch_Ntf_ConfirmOk, ntf);
			});
	});
	auto* pUser = gTeamUserMgr->getRole(roleId);
	if (pUser != nullptr)
	{
		assert(pUser->pUnit->room == this);
	}
}

void MatchRoom::setCopyScene(const inner::InnerMatchRoomCopyScene& in)
{
	assert(_copyscene == nullptr);
	_copyscene = new inner::InnerMatchRoomCopyScene(in);
	Log_Info("setCopyScene");
}

void MatchRoom::setCreateBy(const inner::InnerCopyCreateBy& in)
{
	assert(_createby == nullptr);
	_createby = new inner::InnerCopyCreateBy(in);
	Log_Info("setCreateBy");
}

void MatchRoom::startLoading()
{
	Log_Info("startLoading");
	std::list<MatchUnit*> unitList;
	foreach([&](auto& unit)->bool
	{
			unit.startLoading();
			unitList.push_back(&unit);
			return true;
	});
	for (auto* pUnit : unitList)
	{
		pUnit->removeAllMember();
		gMatchUnitMgr->addToRecycle(pUnit);
	}
}

void MatchRoom::forceRecycle()
{
	Log_Info("forceRecycle");
	assert(emptyUnit());
	gMatchRoomMgr->addToRecycle(this);
}

MatchRoomMgr::MatchRoomMgr()
{
}

MatchRoomMgr::~MatchRoomMgr()
{
}

void MatchRoomMgr::final()
{
	for (auto iter = _mrooms.begin(); iter != _mrooms.end();)
	{
		auto del = iter++;
		auto* pRoom = del->second;
		addToRecycle(pRoom);
	}
	__recycle();
}

void MatchRoomMgr::tick()
{
	__recycle();
}

MatchRoom* MatchRoomMgr::createRoom(const inner::InnerMatchRoom& in)
{
	auto* pRoom = new MatchRoom(in);
	if (pRoom == nullptr)
	{
		Log_Error("createRoom.pRoom== nullptr");
		return nullptr;
	}
	if (!_mrooms.emplace(pRoom->getRoomId(), pRoom).second)
	{
		Log_Error("createRoom.emplace==false");
		pRoom->final();
		SAFE_DELETE(pRoom);
		return nullptr;
	}
	Log_Info("createRoom.%lu", pRoom->getRoomId());
	for (auto& p1 : in.groups())
	{
		for (auto& p2 : p1.second.units())
		{
			auto* pUnit = gMatchUnitMgr->getUnit(p2.first);
			if (pUnit != nullptr)
			{
				pRoom->addUnit(pUnit);
			}
		}
	}
	assert(!pRoom->emptyUnit());
	return pRoom;
}

MatchRoom* MatchRoomMgr::getRoom(zRoomIdType roomId)
{
	auto iter = _mrooms.find(roomId);
	if (iter != _mrooms.end())
	{
		return iter->second;
	}
	return nullptr;
}

void MatchRoomMgr::addToRecycle(MatchRoom* pRoom)
{
	assert(pRoom != nullptr);
	assert(pRoom->emptyUnit());
	_mrooms.erase(pRoom->getRoomId());
	_rmrooms.push_back(pRoom);
	Log_Info("addToRecycle");
}

void MatchRoomMgr::forceRecycle()
{
	Log_Info("forceRecycle");
	for (auto iter = _mrooms.begin(); iter != _mrooms.end();)
	{
		auto del = iter++;
		auto* pRoom = del->second;
		pRoom->forceRecycle();
	}
	__recycle();
}

void MatchRoomMgr::__recycle()
{
	if (!_rmrooms.empty())
	{
		for (auto* pRoom : _rmrooms)
		{
			pRoom->final();
			SAFE_DELETE(pRoom);
		}
		_rmrooms.clear();
	}
}