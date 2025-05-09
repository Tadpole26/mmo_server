#pragma once
#include <string>
#include "ztype.h"
#include "util_load_base.h"

//���ط���������
class CConstConfig : public CUtilLoadBase
{
public:
	bool Init();

	bool AddCommonCfg();
	bool AddLogCfg();
	bool AddMysqlCfg();
public:

	uint32_t					m_uiGroupId = 0;
	uint32_t					m_uiPlatId = 0;

	std::string					m_strLogIp = "";
	uint32_t 					m_uiLogPort = 0;

	//mysql 
	std::string					m_strMysqlHost = "";
	std::string					m_strMysqlUser = "";
	std::string					m_strMysqlPasswd = "";
	std::string					m_strMysqlAccName = "";
	uint32						m_uiMysqlPort = 0;
};
