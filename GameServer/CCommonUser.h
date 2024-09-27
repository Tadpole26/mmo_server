#pragma once
#include "CUserInfo.h"
#include "result_code.pb.h"
using namespace ProtoMsg;

class CUserInfo;
class CCommonUser
{
public:
    static CUserInfo* LoadUserInfo(int64_t iUin, ResultCode& eLoadCode);
    static CUserInfo* GetInCacheUserInfo(int64_t iUin);
    static std::vector<std::pair<int, int>> GetCacheUserID(int iCount);


private:
    static CUserInfo* GetUserDataFromDB(int64_t llUid, ResultCode& eLoadCode);
};