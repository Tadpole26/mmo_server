#pragma once

#include "sdk.h"
//视野组件基类
class CSceneScene;
class CmptView
{
public:
	enum
	{
		enStateId_Normal = 0,						//正常状态
		enStateId_Born = 1 << 0,					//出生状态

		enStateId_Death_Min = 1 << 1,				//死亡-最小
		enStateId_Death_Leave = 1 << 2,				//死亡-退场
		enStateId_Death_Fall = 1 << 3,				//死亡-倒下
		enStateId_Death_Normal = 1 << 4,			//死亡-普通
		enStateId_Death_Max = 1 << 14,				//死亡-最大

		enStateId_Hidden_Min = 1 << 15,				//隐身-最小
		enStateId_Hidden_Sys = 1 << 16,				//隐身-系统
		enStateId_Hidden_Skill = 1 << 17,			//隐身-技能
		enStateId_Hidden_Max = 1 << 30,				//隐身-最大

		enStateId_Max = 1 << 31,					//最大
	};
public:
	CmptView();
	virtual ~CmptView();
	virtual bool init() = 0;
	virtual void final();
	virtual uint64 getViewId() = 0;					//角色唯一id/动态npc唯一id
	virtual uint16 getViewType() = 0;
	virtual uint64 getSceneId() = 0;				//场景id
	virtual uint32 getMapID() = 0;					//地图id
	virtual uint32 getSpeed() = 0;					//移动速度
	virtual CSceneScene* getScene() = 0;
	virtual void notifyGatewayFreshScreen() {}
public:
	inline bool hasInScene() { return _inserted; }
	inline void setInScene() { _inserted = true; }
	inline void setOutScene() { _inserted = false; }
	inline const zPos& getPos() const { return _curPos; }
	inline const zPos& getSrcPos() const { return _srcPos; }
	inline const zPos& getDstPos() const { return _dstPos; }
	inline const zPos& getTerPos() const { return _terPos; }
	inline const zPosI& getPosI() const { return _screenId; }
	bool setPos(const zPos& newPos);
public:
	bool addScreen(const zPos& newpos, const WORD circledir);
	bool freshScreen(const zPos& newpos);
	bool removeScreen();
	bool changeScreen(const zPosI old);
	void sendMeToView();
	void sendMeToViewExcept();
	void sendMeToViewDirect(const int direct);
	void removeMeFromView(const zPosI old);
	void removeMeFromViewExcept(const zPosI old);
	void removeMeFromViewRDir(const zPosI old, const int direct);
	void sendChgFieldToView(std::initializer_list<uint32> changes, bool isfight = false);
	uint32 sizeScreenRole();
	uint32 sizeScreenMonster();
	uint32 sizeNineRole();
	uint32 sizeNineMonster();
	void serverResetMove();
public:
	bool isMovingWalking();					//是否正在移动
	bool moveTo(const zPos& oDst);
public:
	inline uint32 getStateId() { return _stateId; }
	void setStateId(const uint32 stateId);
	inline bool isStateIdDeathLeave() { return _stateId == enStateId_Death_Leave; }
	inline bool isStateIdDeathFall() { return _stateId == enStateId_Death_Fall; }
	inline bool isStateIdDeathNormal() { return _stateId == enStateId_Death_Normal; }
	inline bool isStateIdHiddenSys() { return _stateId == enStateId_Hidden_Sys; }
	inline bool isStateIdHiddenSkill() { return _stateId == enStateId_Hidden_Skill;}
protected:
	uint64 _moveTime = 0;				//上次移动的时间戳(ms)
	uint64 _updateTime = 0;				//上次移动事件更新的时间戳
private:
	zPosI _screenId = (uint32)-1;			//屏索引
	bool _inserted = false;					//是否在场景上
	zPos _curPos;							//当前坐标
	zPos _srcPos;							//起始坐标
	zPos _dstPos;							//目的坐标
	zPos _terPos;							//终点坐标
	std::list<zPos> m_lstPath;				//移动路径
	BYTE _smalldir = _DIR_UP;				//坐标方向(8向)
	WORD _circleDir = 0;					//角度方向(360度),和客户端通信用这个
	float _angleDir = 0.0f;					//弧度方向,以x轴正向为0,顺时针增加
private:
	uint32 _stateId = enStateId_Normal;
	uint64 _stateTimeBegin = 0;
};
