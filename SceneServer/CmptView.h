#pragma once

#include "sdk.h"
//��Ұ�������
class CSceneScene;
class CmptView
{
public:
	enum
	{
		enStateId_Normal = 0,						//����״̬
		enStateId_Born = 1 << 0,					//����״̬

		enStateId_Death_Min = 1 << 1,				//����-��С
		enStateId_Death_Leave = 1 << 2,				//����-�˳�
		enStateId_Death_Fall = 1 << 3,				//����-����
		enStateId_Death_Normal = 1 << 4,			//����-��ͨ
		enStateId_Death_Max = 1 << 14,				//����-���

		enStateId_Hidden_Min = 1 << 15,				//����-��С
		enStateId_Hidden_Sys = 1 << 16,				//����-ϵͳ
		enStateId_Hidden_Skill = 1 << 17,			//����-����
		enStateId_Hidden_Max = 1 << 30,				//����-���

		enStateId_Max = 1 << 31,					//���
	};
public:
	CmptView();
	virtual ~CmptView();
	virtual bool init() = 0;
	virtual void final();
	virtual uint64 getViewId() = 0;					//��ɫΨһid/��̬npcΨһid
	virtual uint16 getViewType() = 0;
	virtual uint64 getSceneId() = 0;				//����id
	virtual uint32 getMapID() = 0;					//��ͼid
	virtual uint32 getSpeed() = 0;					//�ƶ��ٶ�
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
	bool isMovingWalking();					//�Ƿ������ƶ�
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
	uint64 _moveTime = 0;				//�ϴ��ƶ���ʱ���(ms)
	uint64 _updateTime = 0;				//�ϴ��ƶ��¼����µ�ʱ���
private:
	zPosI _screenId = (uint32)-1;			//������
	bool _inserted = false;					//�Ƿ��ڳ�����
	zPos _curPos;							//��ǰ����
	zPos _srcPos;							//��ʼ����
	zPos _dstPos;							//Ŀ������
	zPos _terPos;							//�յ�����
	std::list<zPos> m_lstPath;				//�ƶ�·��
	BYTE _smalldir = _DIR_UP;				//���귽��(8��)
	WORD _circleDir = 0;					//�Ƕȷ���(360��),�Ϳͻ���ͨ�������
	float _angleDir = 0.0f;					//���ȷ���,��x������Ϊ0,˳ʱ������
private:
	uint32 _stateId = enStateId_Normal;
	uint64 _stateTimeBegin = 0;
};
