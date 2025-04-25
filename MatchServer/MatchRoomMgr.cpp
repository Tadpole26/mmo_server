#include "matchcommon.pb.h"
#include "innercommon.pb.h"

#include "cMatch.h"

#include "MatchRoom.h"
#include "RoomF55.h"
#include "MatchRoomMgr.h"
#include "MatchQueueMgr.h"

MatchRoomMgr::MatchRoomMgr()
{
}

MatchRoomMgr::~MatchRoomMgr()
{
}

void MatchRoomMgr::final()
{
	for (auto iter = _alls.begin(); iter != _alls.end();)
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
	if (!_alls.empty())
	{
		for (auto iter = _alls.begin(); iter != _alls.end();)
		{
			auto del = iter++;
			auto* pRoom = del->second;
			pRoom->tick();
		}
	}
}


MatchRoom* MatchRoomMgr::createRoom(MatchQueueMgr& mgr, std::string&& formStr)
{
	__autoIdPair();
	auto roomId = cMatch::unionRoomId(mgr.getEproc(), _idpair.first, _idpair.second);
	MatchRoom* pRoom = nullptr;
	switch (mgr.getMtype())
	{
	case cMatch::eMatchF55:			pRoom = new RoomF55(roomId, std::move(formStr), mgr);					break;
	default:						assert(false);															break;
	}
	assert(pRoom != nullptr);
	if (!_alls.emplace(pRoom->getRoomId(), pRoom).second)
	{
		addToRecycle(pRoom);
		return nullptr;
	}
	Log_Info("createRoom,%lu", pRoom->getRoomId());
	return pRoom;
}

MatchRoom* MatchRoomMgr::getRoom(zRoomIdType roomId)
{
	auto iter = _alls.find(roomId);
	if (iter != _alls.end())
	{
		return iter->second;
	}
	return nullptr;
}

void MatchRoomMgr::addToRecycle(MatchRoom* pRoom)
{
	assert(pRoom != nullptr);
	_alls.erase(pRoom->getRoomId());
	_recycles.push_back(pRoom);
	Log_Info("addToRecycle,%lu", pRoom->getRoomId());
}

void MatchRoomMgr::__recycle()
{
	if (!_recycles.empty())
	{
		for (auto* pRoom : _recycles)
		{
			pRoom->final();
			SAFE_DELETE(pRoom);
		}
		_recycles.clear();
	}
}

void MatchRoomMgr::__autoIdPair()
{
	if (_idpair.first != GetCurrTime())
	{
		_idpair.first = GetCurrTime();
		_idpair.second = 0;
	}
	++_idpair.second;
}

void MatchRoomMgr::copyCreateResult(zSceneIdType sceneId, uint32 hashId, const inner::InnerCopyCreateBy& createBy)
{
	auto* pRoom = getRoom(createBy.crit().critid());
	if (pRoom != nullptr)
	{
		//pRoom->setCreateCopyResult(seceneId, hashId, createBy);
	}
	else
		Log_Error("copyCreateResult.%lu", pRoom->getRoomId());
}