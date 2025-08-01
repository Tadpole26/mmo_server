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
		STATUS_NULL = 0,							//未知状态
		STATUS_ReadyToCheck,						//等待syslog服务器验证
		STATUS_CheckingFromLoginSrv,				//进入syslog服务器验证,等待验证结果
		STATUS_Checking,							//syslog服务器验证完成,进入GameServer验证
		STATUS_CheckedOK,							//登录GameServer成功
		STATUS_CheckedFailed,						//登录GameServer失败
		STATUS_Disconnected,						//客户端断开连接
		STATUS_ReKicking,							//重复登录,正在被踢下线
		STATUS_SysKicking,							//正在被踢下线
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
	std::string m_strAccName;				//账号名称
	uint32 m_uiReLogin = 0;
	bool m_bSaveFlag = false;
	uint32 m_uiSequence = 0;				//请求id(唯一值)
	uint32 m_uiSdk = 0;
	//排队用时,检验用时,在线时段
	stStateTime m_stStat;
	uint32 m_uiGameId = 0;					//游戏中生成的服务器编号+DB的编号
	std::string m_strSessionId;
	BeginEndTime m_oBeginTime;
	time_t m_llNextLineUpPushTime = 0;

	bool m_bReKick = false;
	bool m_bAntiAddiction = false;

	uint32 m_uiProvince = 0;				//省份编号
	std::string m_strProvince;				//省份名字
	std::string m_strDevId;					//设备id
	std::string m_strCreateName;			//创建名字
	uint32 m_uiProArea = 0;					//省份所属地区(北上广)
	std::string m_strChannel;
	std::string m_strCityCode;				//国内6位行政区域码
	std::string m_strMsgKey;			
	CEnDeCodeX m_codex;
};