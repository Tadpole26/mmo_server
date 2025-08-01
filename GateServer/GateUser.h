#pragma once
#include "client.pb.h"

#include "svr_client.h"
#include "en_de_code.h"
#include "cUser.h"

class GateUser : public svr_session, public cUser
{
public:
	enum eUserStatus
	{
		STATUS_NULL = 0,							//δ֪״̬
		STATUS_ReadyToCheck,						//�ȴ�syslog��������֤
		STATUS_CheckingFromLoginSrv,				//����syslog��������֤,�ȴ���֤���
		STATUS_Checking,							//syslog��������֤���,����GameServer��֤
		STATUS_CheckedOK,							//��¼GameServer�ɹ�
		STATUS_CheckedFailed,						//��¼GameServerʧ��
		STATUS_Disconnected,						//�ͻ��˶Ͽ�����
		STATUS_ReKicking,							//�ظ���¼,���ڱ�������
		STATUS_SysKicking,							//���ڱ�������
	};

	struct stStateTime
	{
		eUserStatus m_eStatus = STATUS_NULL;
		int64 m_llRecTime = 0;
		int64 m_llOverTime = 0;

		void Clear()
		{
			m_eStatus = STATUS_NULL;
			m_llRecTime = 0;
			m_llOverTime = 0;
		}
	};
public:
	GateUser(zRoleIdType roleId);
	~GateUser();
	bool init();
	void final();

	virtual void handle_msg(const tagMsgHead* pNetMsg);
	virtual void on_disconnect();
	virtual bool Send(const tagMsgHead* pMsg);

	void sendCmdToMe(google::protobuf::Message* pMessage, uint32 module, msg_id_t cmd, uint32 uiSequence, uint32 eCode);
	void sendCmdToMe(const std::string& strMsg, uint32 module, msg_id_t cmd, uint32 uiSequence, uint32 eCode);
	void sendCmdToSceneSvr(const tagMsgHead* pNetMsgHead);

	eUserStatus GetStatus() { return m_stStat.m_eStatus; }
	bool UpdateStatus(eUserStatus eStatus);
public:
	std::string m_strAccName;				//�˺�����
	uint32 m_uiReLogin = 0;
	bool m_bSaveFlag = false;
	uint32 m_uiSequence = 0;				//����id(Ψһֵ)
	uint32 m_uiSdk = 0;
	//�Ŷ���ʱ,������ʱ,����ʱ��
	stStateTime m_stStat;
	uint32 m_uiGameId = 0;					//��Ϸ�����ɵķ��������+DB�ı��
	std::string m_strSessionId;
	BeginEndTime m_oBeginTime;
	time_t m_llNextLineUpPushTime = 0;

	bool m_bReKick = false;
	bool m_bAntiAddiction = false;

	uint32 m_uiProvince = 0;				//ʡ�ݱ��
	std::string m_strProvince;				//ʡ������
	std::string m_strDevId;					//�豸id
	std::string m_strCreateName;			//��������
	uint32 m_uiProArea = 0;					//ʡ����������(���Ϲ�)
	std::string m_strChannel;
	std::string m_strCityCode;				//����6λ����������
	std::string m_strMsgKey;			
	CEnDeCodeX m_codex;
};