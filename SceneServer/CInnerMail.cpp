#include "CInnerMail.h"
#include "msg_module_mail.pb.h"
#include "global_define.h"
#include "CUserInfo.h"

CInnerMail::CInnerMail(CUserInfo* pUserInfo, const std::string& strTitle, const std::string& strSender,
	const std::string& strContent, const std::string& strAttachment)
{
	time_t llCurTime = GetCurrTime();
	pUserInfo->m_stUserBaseInfo.AddInnerMailAddId(1);
	int64_t llNewMailId = MAKE_MOD_ID(pUserInfo->m_stUserBaseInfo.GetInnerMailAddId(), pUserInfo->m_iGroupId);
	m_pUserInfo = pUserInfo;
	m_stMailRecord.m_llUid = pUserInfo->m_llUid;
	m_stMailRecord.m_llId = llNewMailId;
	m_stMailRecord.m_iMailType = ProtoMsg::eMailType_Game;
	m_stMailRecord.m_llSendTime = llCurTime;
	m_stMailRecord.m_llExpiredTime = llCurTime + WEEK_SECONDS;
	m_stMailRecord.m_bIsDelete = false;
	m_stMailRecord.m_bIsReceived = false;
	m_stMailRecord.m_strTitile = strTitle;
	m_stMailRecord.m_strSender = strSender;
	m_stMailRecord.m_strContent = strAttachment;
	m_stMailRecord.m_strAttachment = strAttachment;
}

void CInnerMail::AddInnerMail()
{
	auto iterFind = m_pUserInfo->m_mapUserMail.Find(m_stMailRecord.m_llId);
	if (iterFind != m_pUserInfo->m_mapUserMail.End())
	{
		Log_Error("add inner mail id exist, user_id:%lld|mail_id:%lld", m_pUserInfo->m_llUid, m_stMailRecord.m_llId);
		return;
	}

	typename decltype(m_pUserInfo->m_mapUserMail)::table_value_type stTableValue(m_stMailRecord.m_llId, m_stMailRecord);
	auto stRet = m_pUserInfo->m_mapUserMail.Insert(stTableValue);
	if (!stRet.second)
	{
		Log_Error("add inner mail error, user_id:%lld|mail_id:%lld", m_pUserInfo->m_llUid, m_stMailRecord.m_llId);
		return;
	}
}