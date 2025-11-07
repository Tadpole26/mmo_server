#pragma once
#include "sdk.h"
#include "server_logic_face.h"

//管理所有连接上来的服务
class ClientLogic : public CSvrLogicFace
{
public:
	ClientLogic(void);
	~ClientLogic(void);
public:
	bool netMsgFromScene(DWORD hashId, const tagHostHd& hostHead, inner::InnerFamilysvr innerReq) final;
};
