#include "ModuleFriend.h"
#include "module.pb.h"
#include "recordfriend.pb.h"

ModuleFriend::ModuleFriend() : GameModule(client::enModule_Friend)
{
	m_pGiftVersion = new record::RecordModuleFriend;
}

ModuleFriend::~ModuleFriend()
{
}

void ModuleFriend::final()
{
	m_pGiftVersion->Clear();
	SAFE_DELETE(m_pGiftVersion);
	for (auto iter : mapFriendList)
		SAFE_DELETE(iter.second);
	mapFriendList.clear();
	GameModule::final();
}

bool ModuleFriend::init(GameUser* pUser)
{
	if (!GameModule::init(pUser))
		return false;
	return true;
}

void ModuleFriend::msgRequestFriend(uint64 llRoleId)
{
}

void ModuleFriend::msgAgreeFriend(uint64 llRoleId)
{
}

void ModuleFriend::msgRefuseFriend(uint64 llRoleId)
{
}

void ModuleFriend::msgAgreeAll()
{
}

void ModuleFriend::msgRefuseAll()
{
}

void ModuleFriend::msgRemoveFriend(uint64 llRoleId)
{
}

void ModuleFriend::msgBlockRole(uint64 llRoleId)
{
}

void ModuleFriend::msgSearchRole(const std::string& strNick)
{
}

void ModuleFriend::msgSetFilter(bool bCity, uint32 uiSex, uint32 uiBitLabel)
{
}

void ModuleFriend::msgGetRecommendList()
{
}

void ModuleFriend::msgSetMyLabel(uint32 uiCity, bool bAvailable, uint32 uiBitLabel)
{
}

void ModuleFriend::msgSendGift(uint64 llRecver, uint32 uiItemUid, uint64 llItemCount)
{
}

void ModuleFriend::msgReceiveGift(uint64 llGiftUid)
{
}

bool ModuleFriend::useExpandItem(uint32 uiItemTid, bool bBind, uint32 uiAddValue)
{
	return true;
}

bool ModuleFriend::isMyFriend(uint32 uiRoleId)
{
	return true;
}