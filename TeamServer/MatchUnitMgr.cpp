#include "matchcommon.pb.h"
#include "innermatch.pb.h"

#include "TeamUser.h"
#include "MatchUnitMgr.h"
#include "MatchRoomMgr.h"

MatchUnit::MatchUnit(zUnitIdType unitId, uint32 mtype) :
	_mtype(mtype),
	_unitId(unitId)
{
	_mtime = GetCurrTime();
}

MatchUnit::~MatchUnit()
{
}

void MatchUnit::final()
{
	assert(emptyMember());
	assert(room == nullptr);
}

void MatchUnit::fill(common::MatchStatus& out)
{
	out.set_mtype(getMatchType());
	out.set_mtime(getMatchTime());
}

bool MatchUnit::foreach(std::function<bool(TeamUser& member)> func)
{
	for (auto* pUser : _allmember)
	{
		if (!func(*pUser)) return false;
	}
	return true;
}

void MatchUnit::addMember(TeamUser* pUser)
{
	assert(pUser != nullptr);
	assert(pUser->pUnit == nullptr);
	_allmember.push_back(pUser);
	pUser->pUnit = this;
	Log_Info("addMember.%lu", pUser->getRoleId());
}

void MatchUnit::removeAllMember()
{
	foreach([&](auto& member)->bool
	{
		assert(member.pUnit == this);
		member.pUnit = nullptr;
		return true;
	});
	_allmember.clear();
	if (room != nullptr)
	{
		room->removeUnit(this);
	}
}

void MatchUnit::startMatch()
{
	Log_Info("startMatch");
	foreach([&](auto& member)->bool
		{
			assert(member.pUnit == this);
			member.openMatchPanelNtf();	
			return true;
		});
}

void MatchUnit::restartMatch()
{
	Log_Info("restartMatch");
	assert(room != nullptr);
	foreach([&](auto& member)->bool 
	{
			assert(member.pUnit == this);
			member.closeConfirmPanelNtf();
			member.openMatchPanelNtf();
			return true;
	});
	room->removeUnit(this);
}

void MatchUnit::restartMatchFromQueue()
{
	assert(room == nullptr);
	Log_Info("restartMatchFromQueue");
	_mtime = GetCurrTime();
	foreach([&](auto& member)->bool 
	{
			assert(member.pUnit == this);
			//systips.send(member, Tips_Match_AreanaStartNextRound);
			member.closeMatchPanelNtf();
			member.openMatchPanelNtf();
			return true;
	});
}

void MatchUnit::cancelMatch()
{
	Log_Info("cancelMatch");
	foreach([&](auto& member)->bool 
		{
			assert(member.pUnit == this);
			member.closeMatchPanelNtf();
			return true;
		});
	removeAllMember();
	gMatchUnitMgr->addToRecycle(this);
}

void MatchUnit::punishMatch(const inner::InnerMatchPunishList& in)
{
	auto funcFind = ([&](zRoleIdType roleId)->bool
	{
			for (auto i = 0; i < in.roleids_size(); ++i)
			{
				if (in.roleids(i) == roleId) return true;
			}
			return false;
	});
	assert(room != nullptr);
	Log_Info("punishMatch");
	foreach([&](auto& member)->bool 
	{
			assert(member.pUnit == this);
			if (funcFind(member.getRoleId()))
			{
				member.closeConfirmPanelNtf();
				member.setMatchPunish(cMatch::ePunishRefuse);
			}
			else
			{
				member.closeConfirmPanelNtf();
			}
			return true;
	});
	removeAllMember();
	gMatchUnitMgr->addToRecycle(this);
}

void MatchUnit::startConfirm()
{
	Log_Info("startConfirm");
	foreach([&](auto& member)->bool
	{
		assert(member.pUnit == this);
		return true;
	});
}

void MatchUnit::startLoading()
{
	Log_Info("startLoading");
	foreach([&](auto& member)->bool
	{
			assert(member.pUnit == this);
			//member.jumpCopy(room->getCopyScene().sceneid(), room->getCreateBy().copy());
			member.closeConfirmPanelNtf();
			return true;
	});
}

void MatchUnit::forceCancelWithEv()
{
	Log_Info("forceCancelWithEv");
	auto funcGetCancelType = ([&]()->uint32 
	{
		if (room == nullptr) return 0;
		else				 return 1;
	});
	auto type = funcGetCancelType();
	foreach([&](auto& member)->bool
	{
			assert(member.pUnit == this);
			switch (type)
			{
			case 0:
				member.closeMatchPanelNtf();
				break;
			case 1:
				member.closeConfirmPanelNtf();
				break;
			}
			return true;
	});
	removeAllMember();
	gMatchUnitMgr->addToRecycle(this);
}

void MatchUnit::forceCancel()
{
	Log_Info("forceCancel");
	foreach([&](auto& member)->bool 
	{
			assert(member.pUnit == this);
			return true;
	});
}

MatchUnitMgr::MatchUnitMgr()
{
}

MatchUnitMgr::~MatchUnitMgr()
{
}

void MatchUnitMgr::final()
{
	forceCancel();
}

void MatchUnitMgr::tick()
{
}

MatchUnit* MatchUnitMgr::createUnit(zUnitIdType unitId, uint32 mtype)
{
	auto* pUnit = new MatchUnit(unitId, mtype);
	if (pUnit == nullptr)
	{
		Log_Error("createUnit.pUnit==nullptr,unitId:%lu,mtype:%u", unitId, mtype);
		return nullptr;
	}
	if (!_munits.emplace(pUnit->getUnitId(), pUnit).second)
	{
		Log_Error("createUnit.emplace==false,unitId:%lu,mtype:%u", unitId, mtype);
		pUnit->final();
		SAFE_DELETE(pUnit);
		return nullptr;
	}
	Log_Info("createUnit,%lu", pUnit->getUnitId());
	return pUnit;
}

MatchUnit* MatchUnitMgr::getUnit(zUnitIdType unitId)
{
	auto iter = _munits.find(unitId);
	if (iter != _munits.end())
	{
		return iter->second;
	}
	return nullptr;
}

void MatchUnitMgr::addToRecycle(MatchUnit* pUnit)
{
	assert(pUnit != nullptr);
	assert(pUnit->emptyMember());
	assert(pUnit->room == nullptr);
	_munits.erase(pUnit->getUnitId());
	_rmunits.push_back(pUnit);
	Log_Info("addToRecycle,%lu", pUnit->getUnitId());
}

void MatchUnitMgr::forceCancelWithEv()
{
	Log_Info("forceCancelWithEv");
	for (auto iter = _munits.begin(); iter != _munits.end();)
	{
		auto del = iter++;
		auto* pUnit = del->second;
		pUnit->forceCancelWithEv();
	}
	__recycle();
}

void MatchUnitMgr::forceCancel()
{
	for (auto iter = _munits.begin(); iter != _munits.end();)
	{
		auto del = iter++;
		auto* pUnit = del->second;
		pUnit->forceCancel();
	}
	__recycle();
}