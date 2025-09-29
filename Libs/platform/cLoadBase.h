#pragma once
#include <string>
#include <functional>
#include <map>
#include "sdk.h"

class cLoadBase
{
public:
	virtual ~cLoadBase() { }

	using fn_read = std::function<bool(void)>;

	struct sheet
	{
		bool bCanEmpty = false;
		bool bInit = false;
		uint32_t uiLines = 0;
		fn_read pFnRead = nullptr;
	};

	void InitSheet(const char* pSheet, fn_read fn, bool bCanEmpty = false);
	bool IsValid();
	//��xml�ļ�����
	bool LoadCfgFromXml(const char* pXmlFileName = nullptr);
	//��string����
	bool LoadCfgFromStr(const std::string& strCfg);
	virtual bool LoadConfig(const std::string& strContent) = 0;
	virtual bool LoadConfigByFile(const std::string& fileName) = 0;

	bool ReadString(const char* pField, std::string& strRet);
	std::string GetDataChr(const char* pField, const std::string& strDef = "");
	int32_t GetDataInt(const char* pField, const int32_t iDef = 0);
	bool GetDataBool(const char* pField, const bool bDef = false);
	uint32_t GetDataUInt(const char* pField, const uint32_t uiDef = 0);
	double GetDataDouble(const char* pField, const double dDef = 0.0f);
	time_t GetDataTime(const char* pField, const time_t tDef = 0);
	int64_t GetDataInt64(const char* pField, const int64_t llDef = 0);
	bool GetDataItem(const char* pField, std::map<uint32, uint32>& mapResult);
	bool GetDataVec(const char* pField, std::vector<uint32>& vecResult);
	bool GetDataPos(const char* pField, zPos& oResult);
	bool GetDataPos(const char* pField, std::vector<zPos>& vecResult);

public:
	std::string m_strFile = "";
	std::string m_strCurSheet = "";
	std::map<std::string, std::string> m_mapRowData;
	std::map<std::string, sheet> m_mapSheet;
};
