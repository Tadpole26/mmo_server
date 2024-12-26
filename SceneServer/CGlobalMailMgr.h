#pragma once
#include "singleton.h"
#include <map>
#include "db_struct.h"
#include "CUserInfo.h"


struct global_mail_t
{
	int64_t m_llMailId = 0;
	std::string m_strTitle = "";
	std::string m_strSender = "";
	std::string m_strContent = "";
	std::string m_strAttachment = "";
	time_t m_llCreateTime = 0;
	time_t m_llExpiredTime = 0;
};

class CGlobalMailMgr
{
public:
	//����load����
	bool GetGlobalMailFromDB();
	//gameadmin������ʼ�
	bool GetGlobalMailFromDB(int64_t llMailId);
	//����ϵͳ�ʼ�
	void CheckGlobalMail(CUserInfo* pUserInfo);

private:
	bool ParseGlobalMail(sDBMap& oMainRet);

public:
	std::map<int64_t, global_mail_t> m_mapGlobalMail;
};

#define GLOBAL_MAIL_INS Singleton<CGlobalMailMgr>::getInstance()

