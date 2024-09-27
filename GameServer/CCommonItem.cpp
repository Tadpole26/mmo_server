#include "CCommonItem.h"
#include "CUserInfo.h"
#include "reader.h"
#include "log_mgr.h"
#include "util_strop.h"
#include "singleton.h"
#include "CItemConfig.h"
#include "CUserConfig.h"

bool CCommonItem::ParseItem(const std::string& strItem, std::vector<game_item_t>& vecItem)
{
	if (strItem.empty())
		return false;

	Json::Reader stReader;
	Json::Value stRoot;
	if (!stReader.parse(strItem, stRoot))
	{
		Log_Error("parse item fail, %s!", strItem.c_str());
		return false;
	}

	if (stRoot.isNull() || !stRoot.isArray())
	{
		Log_Error("invalid item data!");
		return false;
	}

	for (auto& value : stRoot)
	{
		if (!value["id"].isIntegral() || !value["num"].isIntegral())
		{
			Log_Error("invalid item data, item is not intefral!");
			return false;
		}
		game_item_t stItem;
		stItem.set_uiid(value["id"].asUInt());
		stItem.set_uinum(value["num"].asUInt());
		vecItem.push_back(stItem);
	}
	return true;
}

bool CCommonItem::AwardItem(CUserInfo* pUserInfo, uint32_t uiItemId, uint32_t uiItemCount, std::vector<game_item_t>& vecOutItem)
{
	if (uiItemCount == 0) return true;

	item_elem_t* pConfig = ITEM_CFG_INS->GetItemElemCfg(uiItemId);
	if (pConfig == nullptr)
	{
		Log_Error("user id:%lld not find item in config, item id:%u!", pUserInfo->m_llUid, uiItemId);
		return false;
	}

	switch (pConfig->m_uiItemType)
	{
	case eItemType_Currency:							//普通货币道具
		return AwardCurrencyItem(pUserInfo, uiItemId, uiItemCount, vecOutItem);
		break;
	case eItemType_UserExp:								//玩家经验
		return AwardUserExpItem(pUserInfo, uiItemId, uiItemCount, vecOutItem);
		break;
	case eItemType_VipExp:								//玩家vip经验
		return AwardVipExpItem(pUserInfo, uiItemId, uiItemCount, vecOutItem);
		break;
	case eItemType_HeroSoul:							//玩家英雄碎片
		return AwardHeroSoulItem(pUserInfo, uiItemId, uiItemCount, vecOutItem);
		break;
	}

	return true;
}

bool CCommonItem::AwardItem(CUserInfo* pUserInfo, std::vector<game_item_t>& vecItem, std::vector<game_item_t>& vecOutItem)
{
	for (auto& item : vecItem)
	{
		return AwardItem(pUserInfo, item.uiid(), item.uinum(), vecOutItem);
	}
	return true;
}

bool CCommonItem::AwardCurrencyItem(CUserInfo* pUserInfo, uint32_t uiItemId, uint32_t uiItemCount, std::vector<game_item_t>& vecOutItem)
{
	time_t llItemTime = GetCurrTime();
	auto stIter = pUserInfo->m_mapUserItem.Find(uiItemId);
	if (stIter == pUserInfo->m_mapUserItem.End())
	{
		user_item_table_value_type stItemValue;
		stItemValue.m_iId = uiItemId;
		stItemValue.m_iItemNum = uiItemCount;
		stItemValue.m_llItemTime = llItemTime;
		stItemValue.m_llUid = pUserInfo->m_llUid;

		typename decltype(pUserInfo->m_mapUserItem)::table_value_type stTableValue(stItemValue.m_iId, stItemValue);
		auto stRet = pUserInfo->m_mapUserItem.Insert(stTableValue);
		if (!stRet.second)
		{
			Log_Error("award currency item insert error, user_id:%lld|item_id:%u|item_count:%u", pUserInfo->m_llUid, uiItemId, uiItemCount);
			return false;
		}
	}
	else
	{
		llItemTime = stIter->second->GetItemTime();
		stIter->second->AddItemNum(uiItemCount);
	}
	game_item_t stOutItem;
	stOutItem.set_uiid(uiItemId);
	stOutItem.set_uinum(uiItemCount);
	stOutItem.set_lltime(llItemTime);
	vecOutItem.push_back(stOutItem);
	return true;
}

bool CCommonItem::AwardUserExpItem(CUserInfo* pUserInfo, uint32_t uiItemId, uint32_t uiItemCount, std::vector<game_item_t>& vecOutItem)
{
	uint32_t uiCurLevel = pUserInfo->m_stUserBaseInfo.GetUserLevel();
	uint32_t uiCurExp = pUserInfo->m_stUserBaseInfo.GetUserExp();
	std::map<uint32_t, user_level_elem_t>& mapUserLevelCfg = USER_CFG_INS->GetUserLevelMapCfg();
	if (mapUserLevelCfg.size() == 0)
	{
		Log_Error("award user exp item config error, use_id:%lld|item_id:%u|item_count:%u", pUserInfo->m_llUid, uiItemId, uiItemCount);
		return false;
	}
	uint32_t uiMaxLevel = mapUserLevelCfg.rbegin()->first;
	uiCurExp += uiItemCount;
	for (uint32_t i = uiCurLevel; i < uiMaxLevel; ++i)
	{
		user_level_elem_t* pExpCfg = USER_CFG_INS->GetUserLevelElemCfg(i);
		if (pExpCfg == nullptr)
		{
			Log_Error("award user exp item config error, use_id:%lld|item_id:%u|item_count:%u", pUserInfo->m_llUid, uiItemId, uiItemCount);
			return false;
		}
		if (uiCurExp < pExpCfg->uiExp)
			break;

		uiCurLevel++;
		uiCurExp -= pExpCfg->uiExp;
	}
	//增加经验升级
	if (pUserInfo->m_stUserBaseInfo.GetUserLevel() != uiCurLevel)
	{
		pUserInfo->m_stUserBaseInfo.SetUserLevel(uiCurLevel);
	}
	pUserInfo->m_stUserBaseInfo.SetUserExp(uiCurExp);
	return true;
}

bool CCommonItem::AwardVipExpItem(CUserInfo* pUserInfo, uint32_t uiItemId, uint32_t uiItemCount, std::vector<game_item_t>& vecOutItem)
{
	uint32_t uiCurVip = pUserInfo->m_stUserBaseInfo.GetVipLevel();
	uint32_t uiCurExp = pUserInfo->m_stUserBaseInfo.GetVipExp();
	std::map<uint32_t, user_vip_elem_t>& mapUserVipCfg = USER_CFG_INS->GetUserVipMapCfg();
	if (mapUserVipCfg.size() == 0)
	{
		Log_Error("award user exp item config error, use_id:%lld|item_id:%u|item_count:%u", pUserInfo->m_llUid, uiItemId, uiItemCount);
		return false;
	}
	uint32_t uiMaxVip = mapUserVipCfg.rbegin()->first;
	uiCurExp += uiItemCount;
	for (uint32_t i = uiCurVip; i < uiMaxVip; ++i)
	{
		user_level_elem_t* pExpCfg = USER_CFG_INS->GetUserLevelElemCfg(i);
		if (pExpCfg == nullptr)
		{
			Log_Error("award user exp item config error, use_id:%lld|item_id:%u|item_count:%u", pUserInfo->m_llUid, uiItemId, uiItemCount);
			return false;
		}
		if (uiCurExp < pExpCfg->uiExp)
			break;

		uiCurVip++;
		uiCurExp -= pExpCfg->uiExp;
	}
	//增加经验升级
	if (pUserInfo->m_stUserBaseInfo.GetVipLevel() != uiCurVip)
	{
		pUserInfo->m_stUserBaseInfo.SetVipLevel(uiCurVip);
	}
	pUserInfo->m_stUserBaseInfo.SetVipExp(uiCurExp);
	
	return true;
}

bool CCommonItem::AwardHeroSoulItem(CUserInfo* pUserInfo, uint32_t uiItemId, uint32_t uiItemCount, std::vector<game_item_t>& vecOutItem)
{
	return true;
}