#include "CGlobalMailMgr.h"
#include "CDbInstance.h"
#include "CUserInfo.h"
#include "CUserMailRecord.h"
#include "CGameConstConfig.h"
#include "msg_module_mail.pb.h"
#include "CMailProcessor.h"

bool CGlobalMailMgr::GetGlobalMailFromDB()
{
	sDBSelectReq m_stMailSelect;
	m_stMailSelect.Init("g_mail_list");
	int64_t llCount = GAME_DB_INS->m_oMailQuery.DBCount(m_stMailSelect);
	size_t uiRetNum = 0;
	fn_RetCall fnCall = [&](sDBRet& oRet)->bool
	{
		global_mail_t stGlobalElem;
		sDBMap& oMainRet = oRet.Ret_Main();
		if (!ParseGlobalMail(oMainRet))
			return false;
		return true;
	};

	int64_t llBeginId = 0;
	while (true)
	{
		if (!GAME_DB_INS->m_oMailQuery.DBSelectMore(m_stMailSelect, fnCall, uiRetNum, llBeginId, 100))
		{
			Log_Error("select global mail fail!");
			return false;
		}

		SetCurrTime();
		if (uiRetNum < 100)
			break;
	}
	Log_Custom("start", "loading global mail from db %lld|%u", llCount, uiRetNum);
	return true;
}

bool CGlobalMailMgr::GetGlobalMailFromDB(int64_t llMailId)
{
	sDBSelectReq m_stMailSelect;
	m_stMailSelect.Init(GLOBAL_MAIL_LIST);
	m_stMailSelect.AddCon("_id", llMailId);
	if (!GAME_DB_INS->m_oMailQuery.DBSelectOne(m_stMailSelect))
	{
		Log_Error("select global mail fail, mail_id:%lld", llMailId);
		return false;
	}

	if (m_stMailSelect.empty_ret())
	{
		Log_Error("select global mail empty, mail_id:%lld", llMailId);
		return false;
	}

	sDBMap& oMainRet = m_stMailSelect.Ret_Main();
	if (!ParseGlobalMail(oMainRet))
		return false;

	//在线玩家通知
	for (auto iter = g_UserInfoLRUHashmap->Begin(); iter != g_UserInfoLRUHashmap->End(); ++iter)
	{
		CheckGlobalMail(iter->second);
	}
	return true;
}

void  CGlobalMailMgr::CheckGlobalMail(CUserInfo* pUserInfo)
{
	//系统新邮件检测
	time_t llCurTime = GetCurrTime();
	bool bNotify = false;
	for (auto iter = m_mapGlobalMail.begin(); iter != m_mapGlobalMail.end(); ++iter)
	{
		if (llCurTime > iter->second.m_llExpiredTime || llCurTime < iter->second.m_llCreateTime)
			continue;

		auto iterFind = pUserInfo->m_mapUserMail.Find(iter->second.m_llMailId);
		if (iterFind != pUserInfo->m_mapUserMail.End())
			continue;

		user_mail_table_value_type stMailValue;
		stMailValue.m_llId = iter->second.m_llMailId;
		stMailValue.m_iMailType = ProtoMsg::eMailType_System;
		stMailValue.m_llSendTime = iter->second.m_llCreateTime;
		stMailValue.m_llExpiredTime = iter->second.m_llExpiredTime;
		stMailValue.m_bIsDelete = false;
		stMailValue.m_bIsReceived = false;
		stMailValue.m_strTitile = iter->second.m_strTitle;
		stMailValue.m_strSender = iter->second.m_strSender;
		stMailValue.m_strContent = iter->second.m_strContent;
		stMailValue.m_strAttachment = iter->second.m_strAttachment;

		typename decltype(pUserInfo->m_mapUserMail)::table_value_type stTableValue(stMailValue.m_llId, stMailValue);
		auto stRet = pUserInfo->m_mapUserMail.Insert(stTableValue);
		if (!stRet.second)
		{
			Log_Error("add global mail error, user_id:%lld|mail_id:%u", pUserInfo->m_llUid, stMailValue.m_llId);
			return;
		}
		bNotify = true;
	}

	//邮件过期、内部邮件手动删除、内部邮件已领取
	for (auto iter = pUserInfo->m_mapUserMail.Begin(); iter != pUserInfo->m_mapUserMail.End();)
	{
		if (llCurTime > iter->second->GetExpiredTime() ||
			(iter->second->GetMailType() == ProtoMsg::eMailType_Game && iter->second->GetIsDelete()) ||
			(iter->second->GetMailType() == ProtoMsg::eMailType_Game && iter->second->GetIsReceived()))
		{
			bNotify = true;
			pUserInfo->m_mapUserMail.Erase(iter++);
		}
		else
			++iter;
	}
	//发送邮件通知
	if (bNotify)
	{
		CMailProcessor::UpdateMailNotify(pUserInfo);
	}
}

bool CGlobalMailMgr::ParseGlobalMail(sDBMap& oMainRet)
{
	global_mail_t stGlobalElem;
	stGlobalElem.m_llMailId = oMainRet.NumLong("_id");
	stGlobalElem.m_strTitle = oMainRet.Str("title");
	stGlobalElem.m_strSender = oMainRet.Str("sender");
	stGlobalElem.m_strContent = oMainRet.Str("content");
	stGlobalElem.m_llCreateTime = oMainRet.Time("begin_date");
	stGlobalElem.m_llExpiredTime = oMainRet.Time("end_date");
	m_mapGlobalMail[stGlobalElem.m_llMailId] = stGlobalElem;
	return true;
}