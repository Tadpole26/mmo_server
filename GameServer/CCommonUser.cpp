#include "CCommonUser.h"
#include "CDbInstance.h"
#include "CGameLogic.h"
#include "CUserInfo.h"

CUserInfo*
CCommonUser::LoadUserInfo(int64_t iUin, ResultCode& eLoadCode)
{
    auto stUserDetailIT = g_UserInfoLRUHashmap->Find(iUin);
    if (stUserDetailIT != g_UserInfoLRUHashmap->End())
    {
        return (stUserDetailIT->second);
    }
    else
    {
        return (GetUserDataFromDB(iUin, eLoadCode));
    }
}

CUserInfo*
CCommonUser::GetInCacheUserInfo(int64_t iUin)
{
    auto stUserDetailIT = g_UserInfoLRUHashmap->Find(iUin);
    if (stUserDetailIT != g_UserInfoLRUHashmap->End())
    {
        return (stUserDetailIT->second);
    }

    return (nullptr);
}

CUserInfo*
CCommonUser::GetUserDataFromDB(int64_t llUid, ResultCode& eLoadCode)
{
    static int64_t llStartTime = GetCurrTime();
    static int64_t llEndTime = 0;
    static int64_t llCostTime = 0;

    sDBSelectReq m_stUserSelect;
    m_stUserSelect.Init(GAME_LOGIC_INS->m_oConstCfg.m_strMongoGameDb);
    m_stUserSelect.AddCon("_id", llUid);
    if (!GAME_DB_INS->QueryData(m_stUserSelect))
    {
        eLoadCode = ResultCode::Code_Common_Failure;
        Log_Error("load user data from mongo db error! user id:%lld", llUid);
        return nullptr;
    }

    if (m_stUserSelect.empty_ret())
    {
        eLoadCode = ResultCode::Code_Login_NoRole;
        Log_Debug("load user is not user, please create user id:%lld", llUid);
        return nullptr;
    }
    sDBRet& stSelectRet = m_stUserSelect.Ret();
    CUserInfo* pUserInfo = new CUserInfo();
    pUserInfo->m_llUid = llUid;
    pUserInfo->m_iGroupId = GAME_LOGIC_INS->m_oConstCfg.m_uiGroupId;
    //玩家基础信息
    std::vector<user_info_table_value_type> vecBaseList = CUserInfoRecord::SelectDataFromDB(llUid, stSelectRet.Ret_Vec(CUserInfoRecord::s_strSecName));
    if (vecBaseList.size() > 0)
        pUserInfo->m_stUserBaseInfo.InitializeWithoutSQL(vecBaseList[0]);
    //玩家英雄列表
    std::vector<user_card_table_value_type> vecHeroList = CUserCardRecord::SelectDataFromDB(llUid, stSelectRet.Ret_Vec(CUserCardRecord::s_strSecName));
    for (auto stIter = vecHeroList.begin(); stIter != vecHeroList.end(); ++stIter)
        pUserInfo->m_mapUserCard.InsertWithoutSQL(std::make_pair(stIter->m_iId, *stIter));
    //玩家道具列表
    std::vector<user_item_table_value_type> vecItemList = CUserItemRecord::SelectDataFromDB(llUid, stSelectRet.Ret_Vec(CUserItemRecord::s_strSecName));
    for (auto stIter = vecItemList.begin(); stIter != vecItemList.end(); ++stIter)
        pUserInfo->m_mapUserItem.InsertWithoutSQL(std::make_pair(stIter->m_iId, *stIter));
    //玩家邮件列表
    std::vector<user_mail_table_value_type> vecMailList = CUserMailRecord::SelectDataFromDB(llUid, stSelectRet.Ret_Vec(CUserMailRecord::s_strSecName));
    for (auto stIter = vecMailList.begin(); stIter != vecMailList.end(); ++stIter)
        pUserInfo->m_mapUserMail.InsertWithoutSQL(std::make_pair(stIter->m_llId, *stIter));

    g_UserInfoLRUHashmap->Insert(std::make_pair(llUid, pUserInfo));
    llEndTime = GetCurrTime();
    llCostTime = llEndTime - llStartTime;
    Log_Debug("get user info from db|uid:%lld|group_id:%d|cache total user:%u|cost time:%lld",
        llUid, pUserInfo->m_iGroupId, g_UserInfoLRUHashmap->Size(), llCostTime);
    return pUserInfo;
}