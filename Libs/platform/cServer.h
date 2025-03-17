#pragma once

#include "sdk.h"

//�������
enum
{
	eProcThreeLocal = 0,					//����
	eProcThreeGroup = 1,					//����
	eProcThreeMax = 2						//������
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
