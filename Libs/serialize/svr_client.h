#pragma once
#include "logic_interface.h"
#include "tcp_client.h"
#include "msg_make.h"
#include "global_define.h"
/*
client�ṹ
(����GateServer��˵�����)
(����GameServer��˵��GateServer/WorldServer�ȵ�)
*/


class svr_session : public CTcpSession, public CMsgMake
{
public:
	svr_session();
	~svr_session();

	virtual bool sendMsg(google::protobuf::Message* pMsg, msg_id_t moduleId, uint16 cmdId = 0);
	virtual bool sendMsg(const std::string& strMsg, msg_id_t moduleId, uint16 cmdId = 0);
	virtual bool sendMsg(const uchar* buf, size_t size, msg_id_t moduleId, uint16 cmdId = 0);

	virtual void Init();
	virtual void Release();
	virtual bool Send(const tagMsgHead* pMsg);

	uint32 GetServerId() const { return m_uiServerId; }
	void SetServerId(uint32 val) { m_uiServerId = val; }

	SERVER_KIND GetServerKind() const { return m_eServerKind; }
	void SetServerKind(SERVER_KIND val) { m_eServerKind = val; }

protected:
	virtual void handle_msgv(const void* pMsg);
	virtual void handle_msg(const tagMsgHead* pMsg) = 0;

	uint32					m_uiServerId = 0;
	SERVER_KIND				m_eServerKind = SERVER_KIND_NONE;
};

class svr_reconn : public CTcpReconn, public CMsgMake
{
public:
	svr_reconn(bool bCache = true, uint32 CacheSec = DEF_CACHE_SECOND);
	~svr_reconn();

	virtual bool sendMsg(google::protobuf::Message* pMsg, msg_id_t moduleId, uint16 cmdId = 0);
	virtual bool sendMsg(const std::string& strMsg, msg_id_t moduleId, uint16 cmdId = 0);
	virtual bool sendMsg(const uchar* buf, size_t size, msg_id_t moduleId, uint16 cmdId = 0);

	virtual bool Send(const tagMsgHead* pMsg);

	uint32 GetServerID() const { return m_dwServerID; }
	void SetServerID(uint32 val) { m_dwServerID = val; }

protected:
	virtual void handle_msgv(const void* pMsg);
	virtual void handle_msg(const tagMsgHead* pMsg) = 0;
	uint32 m_dwServerID = 0;
};