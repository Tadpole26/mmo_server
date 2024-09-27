#include "CUserInfo.h"
#include "msg_module_servercommon.pb.h"
#include "CGameLogic.h"

bool CUserInfo::SendToClient(uint32_t uiRspModule, uint32_t uiRspCmd, google::protobuf::Message* pRspMsg, ProtoMsg::ResultCode eCode, bool bNotify)
{
    ProtoMsg::Msg_ServerCommon_Transmit oTransmit;

    oTransmit.set_lluid(this->m_llUid);
    oTransmit.set_uimoduleid(uiRspModule);
    oTransmit.set_uicmd(uiRspCmd);
    oTransmit.set_uiretcode(eCode);
    oTransmit.set_uisequence(bNotify ? 0 : this->m_iSequence);
    oTransmit.set_strcmdmsg(pRspMsg->SerializeAsString());
    if (GAME_LOGIC_INS->m_pGateSession)
        GAME_LOGIC_INS->m_pGateSession->Send_Msg(&oTransmit, MsgModule_ServerCommon::Msg_ServerCommon_Transmit);
    return true;
}