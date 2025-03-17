#include "CSceneMapMgr.h"
#include "log_mgr.h"
#include "util_string.h"
#include "util_strop.h"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

CSceneMapMgr::CSceneMapMgr()
{
}

CSceneMapMgr::~CSceneMapMgr()
{
}

void CSceneMapMgr::Final()
{
}

bool CSceneMapMgr::Init()
{
}

bool CSceneMapMgr::__ScanAllBrushFiles()
{
	std::string strPath = "design/mapcfg/";
	if (!fs::exists(strPath) || !fs::is_directory(strPath))
	{
		Log_Error("%s not exist", strPath.c_str());
		return false;
	}

	fs::recursive_directory_iterator iter(strPath);
	fs::recursive_directory_iterator endIter;
	while (iter != endIter)
	{
		if (fs::is_regular_file(*iter))
		{
			std::string strFileName = iter->path().filename().string();
			std::vector<std::string> vecResult;
			if (str_split(strFileName, ',', vecResult) == 0)
			{
				Log_Error("%s name not valid", strFileName.c_str());
				return false;
			}
			std::vector<uint32> vecFileId;
			if (str_split_num(vecResult[0], ',', vecFileId) != 2)
			{
				Log_Error("%s name not valid", strFileName.c_str());
				return false;
			}
			if (!m_mapAllBrushFiles[vecFileId[0]].insert(vecFileId[1]).second)
			{
				Log_Error("%s name id duplicate", strFileName.c_str());
				return false;
			}
			Log_Info("insert bursh file %s", strFileName.c_str());
		}
	}
	return true;
}	




void CSceneMapMgr::__CleanAllBrushFiles()
{
	m_mapAllBrushFiles.clear();
}