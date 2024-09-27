#include "CLoginProcessor.h"
#include "CModuleProFactory.h"
#include "msg_module.pb.h"
#include "msg_module_login.pb.h"
#include "result_code.pb.h"
#include "CUserInfo.h"
#include "CItemConfig.h"
#include "CCommonItem.h"
using namespace ProtoMsg;

REGISTER_MODULE_FACTORY(MsgModule::Login, CLoginProcessor)

bool
CLoginProcessor::DoUserRun(const std::string& strMsg)
{
    if (m_uiCmd == MsgModule_Login::Msg_Login_GetItemList_Req)
    {
        Msg_Login_GetItemList_Rsp  oItemRsp;
        ResultCode eCode = (ResultCode)GetItemList(strMsg, oItemRsp);
        SendToClient(MsgModule::Login, MsgModule_Login::Msg_Login_GetItemList_Rsp, &oItemRsp, eCode);
    }
    else if (m_uiCmd == MsgModule_Login::Msg_Login_AwardItem_Req)
    {
        Msg_Login_AwardItem_Rsp oAwardRsp;
        ResultCode eCode = (ResultCode)AwardItem(strMsg, oAwardRsp);
        SendToClient(MsgModule::Login, MsgModule_Login::Msg_Login_AwardItem_Rsp, &oAwardRsp, eCode);
    }
    return (true);
}

int CLoginProcessor::GetItemList(const std::string& strMsg, Msg_Login_GetItemList_Rsp& oItemRsp)
{
    for (auto iter = m_pUserInfo->m_mapUserItem.Begin(); iter != m_pUserInfo->m_mapUserItem.End(); ++iter)
    {
        game_item_t* pMsgItem = oItemRsp.add_vecitems();
        if (pMsgItem == nullptr) continue;
        pMsgItem->set_uiid(iter->second->GetId());
        pMsgItem->set_uinum(iter->second->GetItemNum());
        pMsgItem->set_lltime(iter->second->GetItemTime());
    }
    return Code_Common_Success;
}

int CLoginProcessor::AwardItem(const std::string& strMsg, Msg_Login_AwardItem_Rsp& oAwardRsp)
{
    Msg_Login_AwardItem_Req oAwardReq;
    PARSE_PTL_STR_RET_FAIL(oAwardReq, strMsg);

    item_elem_t* pItemCfg = ITEM_CFG_INS->GetItemElemCfg(oAwardReq.uiitemid());
    if (pItemCfg == nullptr)
    {
        Log_Error("item not in config, item_id:%u", oAwardReq.uiitemid());
        return Code_Common_Failure;
    }
    std::vector<game_item_t> vecOutItem;
    if(!CCommonItem::AwardItem(m_pUserInfo, oAwardReq.uiitemid(), oAwardReq.uicount(), vecOutItem))
    {
        Log_Error("item award error, item_id:%u", oAwardReq.uiitemid());
        return Code_Common_Failure;
    }

    oAwardRsp.set_uiitemid(oAwardReq.uiitemid());
    oAwardRsp.set_uicount(oAwardReq.uicount());
    return Code_Common_Success;
}





