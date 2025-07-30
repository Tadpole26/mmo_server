#pragma once
#include "sdk.h"
#include "cServer.h"

class cScene
{
public:
	enum enSceneRunState
	{
		enSceneRunState_Create,								//创建
		enSceneRunState_Register,							//注册
		enSceneRunState_Running,							//运行
		enSceneRunState_Recycle,							//等待回收
		enSceneRunState_Remove,								//删除
	};
public:
	static const uint32 FirstMapId = 101;					//新手村地图id
	static const uint32 MapIdMin = 1;
	static const uint32 MapIdMax = 0xFFF;
	static const uint32 CopyLvIdMin = 1;
	static const uint32 CopyLvIdMax = 0xFFFF;
	static const uint32 ProdTypeValMin = 0;
	static const uint32 ProcTypeValMax = eProcThreeMax - 1;
	static const uint32 ProcParamValMin = 1;
	static const uint32 ProcParamValMax = 0xFFFF;
	static const uint32 AutoIdValMin = 1;
	static const uint32 AutoIdValMax = 0xFFFF;

public:
	struct UnionSceneId
	{
		union
		{
			zSceneIdType sceneId;
			struct
			{
				uint32 reserved : 2;
				uint32 eProc : 2;
				uint32 mapId : 12;						//对应配置mapid,配置地图id不能超过12位
				uint32 copyLvId : 16;
				uint32 eParam : 16;
				uint32 autoId : 16;
			};
		};
		UnionSceneId() : sceneId(0) {}
		UnionSceneId(const zSceneIdType sceneId) : sceneId(sceneId) {}
	};
public:
	static bool isValidMapId(const uint32 mapId);
	static bool isValidCopyLvId(const uint32 copyLvId);
	static bool isValidAutoId(const uint32 autoId);
	static uint32 sceneId2MapId(const zSceneIdType sceneId);
	static uint32 sceneId2CopyLvId(const zSceneIdType sceneId);
	static uint32 sceneId2Param(const zSceneIdType sceneId);
	static uint32 sceneId2Eproc(const zSceneIdType sceneId);
	static uint32 sceneId2AutoId(const zSceneIdType sceneId);
	static uint32 sceneId2CopyType(const zSceneIdType sceneId);
	static zSceneIdType unionSceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiCopyLvId, uint32 uiAutoId);
	static zSceneIdType unionFirstSceneId(uint32 eParam, uint32 eProc);
	static zSceneIdType unionStaticSceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiAutoId);
	static zSceneIdType unionCopySceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiCopyLvId, uint32 uiAutoId);
	inline zSceneIdType GetSceneId() const { return _uSid.sceneId; }
	inline uint32 GetMapId() const { return _uSid.mapId; }
	inline uint32 GetEparam() const { return _uSid.eParam; }
	inline uint32 GetEproc() const { return _uSid.eProc; }
	inline uint32 GetCopyLvId() const { return _uSid.copyLvId; }
	inline uint32 GetAutoId() const { return _uSid.autoId; }
public:
	static const char* getCopyTypeStr(const uint32 copyType);
	static const char* getRunStateStr(const enSceneRunState state);
	static const std::string sceneIdStr(const zSceneIdType sceneId);
public:
	cScene(const uint64 ullSceneId);
	virtual ~cScene();
	virtual void final();
	virtual void setRegister();
	virtual void setRunning();
	virtual void setRecycle();
	virtual void setRemove();
public:
	bool isCreate() const { return _runstate == enSceneRunState_Create; }
	bool isRegister() const { return _runstate == enSceneRunState_Register; }
	bool isRunning() const { return _runstate == enSceneRunState_Running; }
	bool isRecycle() const { return _runstate == enSceneRunState_Recycle; }
	bool isRemove() const { return _runstate == enSceneRunState_Remove; }
private:
	void __setRunState(const enSceneRunState state);
private:
	UnionSceneId _uSid;
	enSceneRunState _runstate = enSceneRunState_Create;
}; 
