#pragma once

#include "sdk.h"
//视野组件基类
class CSceneScene;
class CCmptView
{
public:
	CCmptView();
	virtual ~CCmptView();
	virtual bool Init() = 0;
	virtual void Final();
	virtual uint64 GetViewId() = 0;					//角色唯一id/动态npc唯一id
	virtual uint16 GetViewType() = 0;
	virtual uint64 GetSceneId() = 0;				//场景id
	virtual uint32 GetMapID() = 0;					//地图id
	virtual uint32 GetSpeed() = 0;					//移动速度
	virtual CSceneScene* GetScene() = 0;
	virtual void NotifyGatewayFreshScreen() {}
public:
	inline bool HasInScene() { return m_bInserted; }
	inline void SetInScene() { m_bInserted = true; }
	inline void SetOutScene() { m_bInserted = false; }
	inline const zPos& GetPos() const { return m_oCurPos; }
	inline const zPos& GetSrcPos() const { return m_oSrcPos; }
	inline const zPos& GetDstPos() const { return m_oDstPos; }
	inline const zPos& GetTerPos() const { return m_oTerPos; }
	inline const zPosI& GetPosI() const { return m_uiScreenId; }
	bool SetPos(const zPos& newPos);
public:
	bool IsMovingWalking();					//是否正在移动
	bool MoveTo(const zPos& oDst);
protected:
	uint64 m_llMoveTime = 0;				//上次移动的时间戳(ms)
	uint64 m_llUpdateTime = 0;				//上次移动事件更新的时间戳
private:
	zPosI m_uiScreenId = (uint32)-1;		//屏索引
	bool m_bInserted = false;				//是否在场景上
	zPos m_oCurPos;							//当前坐标
	zPos m_oSrcPos;							//起始坐标
	zPos m_oDstPos;							//目的坐标
	zPos m_oTerPos;							//终点坐标
	std::list<zPos> m_lstPath;				//移动路径
	BYTE m_cDir = _DIR_UP;					//坐标方向(8向)
	WORD m_sCircleDir = 0;					//角度方向(360度),和客户端通信用这个
	float m_fAngleDir = 0.0f;				//弧度方向,以x轴正向为0,顺时针增加
};
