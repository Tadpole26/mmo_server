#pragma once

#include "sdk.h"

//�������
enum
{
	eProcThreeLocal = 0,					//three���:����
	eProcThreeGroup = 1,					//three���:����
	eProcThreeMax = 2,						//three������
	eProcZone = eProcThreeMax,				//zone���
	eProcPlat = 3,							//Plat���
	eProcMax = 4							//������
};

//����������
enum
{
	eServerNone = 0,
	eServerMap = 1,			//��ͼ������
	eServerGateway = 2,		//���ط�����
	eServerScene = 3,		//����������
	eServerMax = 4,			//���
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

//�Ƿ���Ч��eProc
inline bool eProcValid(uint32 eProc)
{
	return eProc < eProcMax;
}

//�Ƿ���ЧeProcThree
inline bool eProcThreeValid(uint32 eProc)
{
	return eProc < eProcThreeMax;
}

//�Ƿ���ЧserverType
inline bool eServerValid(uint32 serverType)
{
	return serverType > eServerNone && serverType < eServerMax;
}

//�Ƿ���ЧserverId
inline bool serverIdValid(uint32 serverId)
{
	return serverId > eServerIdNone && serverId < eServerIdMax;
}

//�Ƿ���Ч��groupId
inline bool groupIdValid(uint32 groupId)
{
	return groupId > eGroupIdNone && groupId <= eGroupIdMax;
}

//�Ƿ���Ч��eParam
inline bool eParamValid(uint32 eProc, uint32 eParam)
{
	return true;
}

//����threeId
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

//�Ƿ���Ч��threeId
inline bool threeIdValid(uint32 threeId)
{
	auto eProc = splitEprocThreeId(threeId);
	auto eParam = splitEparamThreeId(threeId);
	return eProcThreeValid(eProc) && eParamValid(eProc, eParam);
}

//����procId
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

//����hashId
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
