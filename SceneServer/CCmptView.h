#pragma once

#include "sdk.h"
//��Ұ�������
class CSceneScene;
class CCmptView
{
public:
	CCmptView();
	virtual ~CCmptView();
	virtual bool Init() = 0;
	virtual void Final();
	virtual uint64 GetViewId() = 0;					//��ɫΨһid/��̬npcΨһid
	virtual uint16 GetViewType() = 0;
	virtual uint64 GetSceneId() = 0;				//����id
	virtual uint32 GetMapID() = 0;					//��ͼid
	virtual uint32 GetSpeed() = 0;					//�ƶ��ٶ�
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
	bool IsMovingWalking();					//�Ƿ������ƶ�
	bool MoveTo(const zPos& oDst);
protected:
	uint64 m_llMoveTime = 0;				//�ϴ��ƶ���ʱ���(ms)
	uint64 m_llUpdateTime = 0;				//�ϴ��ƶ��¼����µ�ʱ���
private:
	zPosI m_uiScreenId = (uint32)-1;		//������
	bool m_bInserted = false;				//�Ƿ��ڳ�����
	zPos m_oCurPos;							//��ǰ����
	zPos m_oSrcPos;							//��ʼ����
	zPos m_oDstPos;							//Ŀ������
	zPos m_oTerPos;							//�յ�����
	std::list<zPos> m_lstPath;				//�ƶ�·��
	BYTE m_cDir = _DIR_UP;					//���귽��(8��)
	WORD m_sCircleDir = 0;					//�Ƕȷ���(360��),�Ϳͻ���ͨ�������
	float m_fAngleDir = 0.0f;				//���ȷ���,��x������Ϊ0,˳ʱ������
};
