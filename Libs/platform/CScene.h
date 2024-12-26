#pragma once
#include "sdk.h"

//地图视野基类
class CScene
{
public:
	static const uint32 FirstMapId = 101;					//新手村地图id
	static const uint32 MapIdMin = 1;
	static const uint32 MapIdMax = 0xFFF;
	static const uint32 AutoIdValMin = 1;
	static const uint32 AutoIdValMax = 0xFFFF;
	static const uint32 CopyLvIdMin = 1;
	static const uint32 CopyLvIdMax = 0xFFFF;
public:
	struct union_scene_id_t
	{
		union
		{
			uint64 ullSceneId;
			struct
			{
				uint32 uiReserved : 2;
				uint32 eProc : 2;
				uint32 uiMapId : 12;						//对应配置mapid,配置地图id不能超过12位
				uint32 uiCopyLvId : 16;
				uint32 eParam : 16;
				uint32 uiAutoId : 16;
			};
		};
		union_scene_id_t() : ullSceneId(0) {}
		union_scene_id_t(const uint64 sceneId) : ullSceneId(sceneId) {}
	};
public:
	static bool IsValidMapId(const uint32 uiMapId);
	static bool IsValidCopyLvId(const uint32 uiCopyLvId);
	static bool IsValidAutoId(const uint32 uiAutoId);
	static uint32 SceneId2MapId(const uint64 ullSceneId);
	static uint32 SceneId2CopyLvId(const uint64 ullSceneId);
	static uint32 SceneId2Param(const uint64 ullSceneId);
	static uint32 SceneId2Eproc(const uint64 ullSceneId);
	static uint32 SceneId2AutoId(const uint64 ullSceneId);
	static uint32 SceneId2CopyType(const uint64 ullSceneId);
	static uint64 UnionSceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiCopyLvId, uint32 uiAutoId);
	static uint64 UnionFirstSceneId(uint32 eParam, uint32 eProc);
	static uint64 UnionStaticSceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiAutoId);
	static uint64 UnionCopySceneId(uint32 eParam, uint32 eProc, uint32 uiMapId, uint32 uiCopyLvId, uint32 uiAutoId);
	inline uint64 GetSceneId() const { return m_uSid.ullSceneId; }
	inline uint32 GetMapId() const { return m_uSid.uiMapId; }
	inline uint32 GetEparam() const { return m_uSid.eParam; }
	inline uint32 GetEproc() const { return m_uSid.eProc; }
	inline uint32 GetCopyLvId() const { return m_uSid.uiCopyLvId; }
	inline uint32 GetAutoId() const { return m_uSid.uiAutoId; }
public:
	CScene(const uint64 ullSceneId);
	virtual ~CScene();
	virtual bool Init() = 0;
	virtual void Final();
private:
	union_scene_id_t m_uSid;
}; 
