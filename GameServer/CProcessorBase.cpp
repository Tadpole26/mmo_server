#include "CProcessorBase.h"
#include "CCommonUser.h"
#include "log_mgr.h"
#include "msg_module_servercommon.pb.h"
#include "CGameLogic.h"

CProcessorBase::CProcessorBase(uint32_t uiModuleId)
	: m_uiModuleId(uiModuleId){ };

CProcessorBase::~CProcessorBase() { };

void CProcessorBase::SetUserBaseInfo(CUserInfo* pUserInfo, uint32_t uiCmd)
{
    m_pUserInfo = pUserInfo;
    m_uiCmd = uiCmd;
}

bool CProcessorBase::DoProcess(const std::string& strMsg)
{
    bool bRet = DoUserRun(strMsg);

    //协议包逻辑处理结束执行db操作
    SQL_TABLE_RECORD_CACHE_INS->ExecSQL();
    return bRet;
}

void CProcessorBase::SendToClient(uint32_t uiRspModule, uint32_t uiRspCmd, google::protobuf::Message* pRspMsg, ResultCode eCode, bool bNotify)
{
    Msg_ServerCommon_Transmit oTransmit;

    oTransmit.set_lluid(m_pUserInfo->m_llUid);
    oTransmit.set_uimoduleid(uiRspModule);
    oTransmit.set_uicmd(uiRspCmd);
    oTransmit.set_uiretcode(eCode);
    oTransmit.set_uisequence(bNotify ? 0 : m_pUserInfo->m_iSequence);
    oTransmit.set_strcmdmsg(pRspMsg->SerializeAsString());
    if (GAME_LOGIC_INS->m_pGateSession)
        GAME_LOGIC_INS->m_pGateSession->Send_Msg(&oTransmit, MsgModule_ServerCommon::Msg_ServerCommon_Transmit);
} 
