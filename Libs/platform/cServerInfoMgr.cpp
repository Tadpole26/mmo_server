#include "innerserver.pb.h"

#include "cGame.h"
#include "cScene.h"
#include "cServerInfoMgr.h"

cGroupInfo::cGroupInfo(const inner::InnerGroupInfo& in)
{
	_info = new inner::InnerGroupInfo(in);
}

cGroupInfo::~cGroupInfo()
{
	SAFE_DELETE(_info);
}

inner::InnerGroupInfo& cGroupInfo::groupInfoRef() const
{
	return *_info;
}

////////////////////////////////////////////////////////////////////////
cZoneInfo::cZoneInfo(const inner::InnerZoneInfo& in)
{
	_info = new inner::InnerZoneInfo(in);
}

cZoneInfo::~cZoneInfo()
{
}

inner::InnerZoneInfo& cZoneInfo::zoneInfoRef() const
{
	return *_info;
}

cGroupInfo& cZoneInfo::groupRef() const
{
	return *_groupinfo;
}

uint32 cZoneInfo::getZoneId() const
{
	return zoneInfoRef().zoneid();
}

void cZoneInfo::setGroupInfo(cGroupInfo* pGroup)
{
	_groupinfo = pGroup;
}
/////////////////////////////////////////////////////////////////////////
cProcessInfo::cProcessInfo(uint32 hashId, const inner::InnerServerInfo& in)
{
	_hashId = hashId;
	_info = new inner::InnerServerInfo(in);
}

cProcessInfo::~cProcessInfo()
{
	SAFE_DELETE(_info);
}

inner::InnerServerInfo& cProcessInfo::infoRef() const
{
	return *_info;
}

cGroupInfo& cProcessInfo::groupRef() const
{
	return *_groupinfo;
}

cZoneInfo& cProcessInfo::zoneRef() const
{
	return *_zoneinfo;
}

uint32 cProcessInfo::getProcEproc() const
{
	return splitEprocHashId(_hashId);
}

uint32 cProcessInfo::getProcEparam() const
{
	return splitEparamHashId(_hashId);
}

uint32 cProcessInfo::getProcServerType() const
{
	return splitEserverHashId(_hashId);
}

uint32 cProcessInfo::getProcServerId() const
{
	return splitServerIdHashId(_hashId);
}

uint32 cProcessInfo::getHashId() const
{
	return _hashId;
}

const std::string& cProcessInfo::getInIp() const
{
	return infoRef().inip();
}

const std::string& cProcessInfo::getOutIp() const
{
	return infoRef().outip();
}

uint32 cProcessInfo::getInPort() const
{
	return infoRef().inport();
}

bool cProcessInfo::isProcLocal() const
{
	return isLocalByEproc(getProcEproc());
}

bool cProcessInfo::isProcZone() const
{
	return isZoneByEproc(getProcEproc());
}

bool cProcessInfo::isProcGroup() const
{
	return isGroupByEproc(getProcEproc());
}

bool cProcessInfo::isProcPlat() const
{
	return isPlatByEproc(getProcEproc());
}

void cProcessInfo::setGroupInfo(cGroupInfo* group)
{
	_groupinfo = group;
}

void cProcessInfo::setZoneInfo(cZoneInfo* zone) 
{
	_zoneinfo = zone;
}
/////////////////////////////////////////////////////////////////////
cServerInfoMgr::cServerInfoMgr()
{
}

cServerInfoMgr::~cServerInfoMgr()
{
	for (auto it = _allgroup.begin(); it != _allgroup.end();)
	{
		auto del = it++;
		auto* pGroup = del->second;
		_allgroup.erase(del);
		SAFE_DELETE(pGroup);
	}
	for (auto it = _allzone.begin(); it != _allzone.end();)
	{
		auto del = it++;
		auto* pZone = del->second;
		_allzone.erase(del);
		SAFE_DELETE(pZone);
	}
	for (auto it = _allserver.begin(); it != _allserver.end();)
	{
		auto del = it++;
		auto* pServer = del->second;
		_allserver.erase(del);
		SAFE_DELETE(pServer);
	}
	_typeserver.clear();
}

size_t cServerInfoMgr::sizeGroup()
{
	return _allgroup.size();
}

size_t cServerInfoMgr::sizeZone()
{
	return _allzone.size();
}

size_t cServerInfoMgr::sizeServer()
{
	return _allserver.size();
}

cGroupInfo* cServerInfoMgr::getGroup(uint32 groupId)
{
	auto it = _allgroup.find(groupId);
	if (it == _allgroup.end()) return nullptr;
	return it->second;
}

cGroupInfo* cServerInfoMgr::addGroup(const inner::InnerGroupInfo& in)
{
	if (!in.groupid())
	{
		Log_Error("addGroup.!groupid");
		return nullptr;
	}
	auto* pGroup = new cGroupInfo(in);
	if (!pGroup)
	{
		Log_Error("addGroup.!pGroup");
		return nullptr;
	}
	if (!_allgroup.emplace(in.groupid(), pGroup).second)
	{
		Log_Error("addGroup.!emplace");
		SAFE_DELETE(pGroup);
		return nullptr;
	}
	Log_Info("addGroup.%s.%u", in.ShortDebugString().c_str(), sizeGroup());
	return pGroup;
}

cZoneInfo* cServerInfoMgr::getZone(uint32 zoneId)
{
	auto it = _allzone.find(zoneId);
	if (it == _allzone.end()) return nullptr;
	return it->second;
}

cZoneInfo* cServerInfoMgr::addZone(const inner::InnerZoneInfo& in)
{
	auto* pGroup = getGroup(in.groupid());
	if (!pGroup)
	{
		Log_Error("addZone.!pGroup.%u", in.groupid());
		return nullptr;
	}
	if (cGame::zoneIdValid(in.zoneid()))
	{
		Log_Error("addZone.!zoneid.%u",in.zoneid());
		return nullptr;
	}
	auto* pZone = new cZoneInfo(in);
	if (!pZone)
	{
		Log_Error("addZone.!pZone");
		return nullptr;
	}
	if (!_allzone.emplace(in.zoneid(), pZone).second)
	{
		Log_Error("addZone.!emplace");
		SAFE_DELETE(pZone);
		return nullptr;
	}
	pZone->setGroupInfo(pGroup);
	Log_Info("addZone.%s.%u", in.ShortDebugString().c_str(), sizeZone());
	return pZone;
}

cProcessInfo* cServerInfoMgr::getProcess(uint32 hashId)
{
	auto it = _allserver.find(hashId);
	if (it == _allserver.end()) return nullptr;
	return it->second;
}

cProcessInfo* cServerInfoMgr::addProcess(uint32 hashId, const inner::InnerServerInfo& in)
{
	auto* pServer = new cProcessInfo(hashId, in);
	if (!_allserver.emplace(hashId, pServer).second)
	{
		Log_Error("addProcess.!emplace.%u", hashId);
		SAFE_DELETE(pServer);
		return nullptr;
	}
	_typeserver[pServer->getProcServerType()].push_back(pServer);
	switch (pServer->getProcEproc())
	{
	case eProcThreeLocal:
	case eProcZone:
		pServer->setZoneInfo(getZone(pServer->getProcEparam()));
		break;
	case eProcThreeGroup:
		pServer->setGroupInfo(getGroup(pServer->getProcEparam()));
		break;
	case eProcPlat:
		break;
	}
	Log_Info("addProcess.%s,eProc:%u,eParam:%u", in.ShortDebugString().c_str(), pServer->getProcEproc(), pServer->getProcEparam());
	return pServer;
}