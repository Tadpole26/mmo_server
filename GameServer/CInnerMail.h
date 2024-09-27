#pragma once
#include "CUserMailRecord.h"

class CUserInfo;
class CInnerMail
{
public:
	CInnerMail(CUserInfo* pUserInfo, const std::string& strTitle, const std::string& strSender,
		const std::string & strContent,const std::string& strAttachment);
	~CInnerMail() = default;

	void AddInnerMail();
private:
	user_mail_table_value_type m_stMailRecord;
	CUserInfo* m_pUserInfo = nullptr;
};
