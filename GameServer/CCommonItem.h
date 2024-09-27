#pragma once
#include <vector>
#include <map>
#include "msg_common.pb.h"
using namespace ProtoMsg;

class CUserInfo;
class CCommonItem
{
public:
	static bool ParseItem(const std::string& strItem, std::vector<game_item_t>& vecItem);

	static bool AwardItem(CUserInfo* pUserInfo, uint32_t uiItemId, uint32_t uiItemCount, std::vector<game_item_t>& vecOutItem);
	static bool AwardItem(CUserInfo* pUserInfo, std::vector<game_item_t>& vecItem, std::vector<game_item_t>& vecOutItem);
	static bool AwardCurrencyItem(CUserInfo* pUserInfo, uint32_t uiItemId, uint32_t uiItemCount, std::vector<game_item_t>& vecOutItem);
	static bool AwardUserExpItem(CUserInfo* pUserInfo, uint32_t uiItemId, uint32_t uiItemCount, std::vector<game_item_t>& vecOutItem);
	static bool AwardVipExpItem(CUserInfo* pUserInfo, uint32_t uiItemId, uint32_t uiItemCount, std::vector<game_item_t>& vecOutItem);
	static bool AwardHeroSoulItem(CUserInfo* pUserInfo, uint32_t uiItemId, uint32_t uiItemCount, std::vector<game_item_t>& vecOutItem);
};



