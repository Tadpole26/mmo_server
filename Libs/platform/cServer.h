#pragma once

#include "sdk.h"

//进程类别
enum
{
	eProcThreeLocal = 0,					//three类别:本服
	eProcThreeGroup = 1,					//three类别:跨组
	eProcThreeMax = 2,						//three类别最大
	eProcZone = eProcThreeMax,				//zone类别
	eProcPlat = 3,							//Plat类别
	eProcMax = 4							//类别最大
};

//服务器类型
enum
{
	eServerNone = 0,
	eServerMap = 1,			//地图服务器
	eServerGateway = 2,		//网关服务器
	eServerScene = 3,		//场景服务器
	eServerMax = 4,			//最大
};

enum
{
	eServerIdNone = 0,
	eServerIdMax = 0xFF,
};

enum
{
	eGroupIdNone = 0,
	eGroupIdMax = 0xFFFF,
};

//是否有效的eProc
inline bool eProcValid(uint32 eProc)
{
	return eProc < eProcMax;
}

//是否有效eProcThree
inline bool eProcThreeValid(uint32 eProc)
{
	return eProc < eProcThreeMax;
}

//是否有效serverType
inline bool eServerValid(uint32 serverType)
{
	return serverType > eServerNone && serverType < eServerMax;
}

//是否有效serverId
inline bool serverIdValid(uint32 serverId)
{
	return serverId > eServerIdNone && serverId < eServerIdMax;
}

//是否有效的groupId
inline bool groupIdValid(uint32 groupId)
{
	return groupId > eGroupIdNone && groupId <= eGroupIdMax;
}

//是否有效的eParam
inline bool eParamValid(uint32 eProc, uint32 eParam)
{
	return true;
}

//计算threeId
inline uint32 unionThreeId(uint32 eProc, uint32 eParam)
{
	assert(eProcThreeValid(eProc));
	assert(eParamValid(eProc, eParam));
	return (eProc << 16) + eParam;
}

inline uint32 splitEprocThreeId(uint32 threeId)
{
	return threeId >> 16;
}

inline uint32 splitEparamThreeId(uint32 threeId)
{
	return (threeId << 16) >> 16;
}

//是否有效的threeId
inline bool threeIdValid(uint32 threeId)
{
	auto eProc = splitEprocThreeId(threeId);
	auto eParam = splitEparamThreeId(threeId);
	return eProcThreeValid(eProc) && eParamValid(eProc, eParam);
}

//计算procId
inline uint32 unionProcId(uint32 eProc, uint32 eParam)
{
	assert(eProcValid(eProc));
	assert(eParamValid(eProc, eParam));
	return (eProc << 16) + eParam;
}

inline uint32 spoitEprocProocId(uint32 procId)
{
	return procId >> 16;
}

inline uint32 splitEparamProcId(uint32 procId)
{
	return (procId << 16) >> 16;
}

//计算hashId
inline uint32 unionHashId(uint32 eProc, uint32 eParam, uint32 eServer, uint32 serverId)
{
	return (eProc << 29) + (eParam << 13) + (eServer << 8) + serverId;
}

inline uint32 splitEprocHashId(uint32 hashId)
{
	return hashId >> 29;
}

inline uint32 splitEparamHashId(uint32 hashId)
{
	return (hashId << 3) >> 16;
}

inline uint32 splitEserverHashId(uint32 hashId)
{
	return (hashId << 19) >> 27;
}

inline uint32 splitServerIdHashId(uint32 hashId)
{
	return (hashId << 24) >> 24;
}

inline bool isLocalByEproc(uint32 eProc)
{
	return eProcThreeLocal == eProc;
}

inline bool isGroupByEproc(uint32 eProc)
{
	return eProcThreeGroup == eProc;
}

inline bool isMapSvr(uint32 eServer) { return eServer == eServerMap; }
inline bool isGatewaySvr(uint32 eServer) { return eServer == eServerGateway; }
inline bool isSceneSvr(uint32 eServer) { return eServer == eServerScene; }

inline bool isSceneSvrLocal(uint32 hashId) { return isLocalByEproc(splitEprocHashId(hashId)) && isSceneSvr(splitEserverHashId(hashId)); }

inline bool isSceneSvrGroup(uint32 hashId) { return isGroupByEproc(splitEprocHashId(hashId)) && isSceneSvr(splitEserverHashId(hashId)); }
