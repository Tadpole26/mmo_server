#pragma once
#include "sdk.h"

namespace inner
{
	class InnerServerInfo;
	class InnerZoneInfo;
	class InnerGroupInfo;
}

//所有group都属于plat
class cGroupInfo
{
public:
	cGroupInfo(const inner::InnerGroupInfo& in);
	~cGroupInfo();
public:
	inner::InnerGroupInfo& groupInfoRef() const;
private:
	inner::InnerGroupInfo* _info = nullptr;
};

//指定区服属于group
class cZoneInfo
{
public:
	cZoneInfo(const inner::InnerZoneInfo& in);
	~cZoneInfo();
public:
	inner::InnerZoneInfo& zoneInfoRef() const;
	cGroupInfo& groupRef() const;
public:
	uint32 getZoneId() const;
public:
	void setGroupInfo(cGroupInfo *pGroup);
private:
	inner::InnerZoneInfo* _info = nullptr;
	cGroupInfo *_groupinfo = nullptr;
};

//进程信息
class cProcessInfo
{
public:
	cProcessInfo(uint32 hashId, const inner::InnerServerInfo& in);
	~cProcessInfo();
public:
	inner::InnerServerInfo& infoRef() const;
	cGroupInfo& groupRef() const;
	cZoneInfo& zoneRef() const;
public:
	uint32 getProcEproc() const;
	uint32 getProcEparam() const;
	uint32 getProcServerType() const;
	uint32 getProcServerId() const;
	uint32 getHashId() const;
	const std::string& getInIp() const;
	const std::string& getOutIp() const;
	uint32 getInPort() const;
public:
	bool isProcLocal() const;
	bool isProcZone() const;
	bool isProcGroup() const;
	bool isProcPlat() const;
public:
	void setGroupInfo(cGroupInfo* group);
	void setZoneInfo(cZoneInfo* zone);
private:
	uint32 _hashId = 0;
	inner::InnerServerInfo* _info = nullptr;//本进程ip:port信息
	cGroupInfo* _groupinfo = nullptr;		//本进程组信息
	cZoneInfo* _zoneinfo = nullptr;			//本进程区信息
};

//当前进程管理的其他进程信息
class cServerInfoMgr
{
public:
	using GroupMapT = std::unordered_map<uint32, cGroupInfo*>;
	using ZoneMapT = std::unordered_map<uint32, cZoneInfo*>;
	using ProcessMapT = std::unordered_map<uint32, cProcessInfo*>;
	using ProcessListT = std::list<cProcessInfo*>;
	using TypeServerMapT = std::unordered_map<uint32, ProcessListT>;//服务器类型
public:
	cServerInfoMgr();
	~cServerInfoMgr();
public:
	size_t sizeGroup();
	size_t sizeZone();
	size_t sizeServer();
public:
	cGroupInfo* getGroup(uint32 groupId);
	cGroupInfo* addGroup(const inner::InnerGroupInfo& in);
	cZoneInfo* getZone(uint32 zoneId);
	cZoneInfo* addZone(const inner::InnerZoneInfo& in);
	cProcessInfo* getProcess(uint32 hashId);
	cProcessInfo* addProcess(uint32 hashId, const inner::InnerServerInfo& in);
private:
	GroupMapT _allgroup;
	ZoneMapT _allzone;
	ProcessMapT _allserver;
	TypeServerMapT _typeserver;
};
