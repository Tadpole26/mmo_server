#include "CModuleFriend.h"
#include "module.pb.h"
#include "recordfriend.pb.h"

CModuleFriend::CModuleFriend() : CModule(client::enModule_Friend)
{
	m_pGiftVersion = new record::RecordModuleFriend;
}

CModuleFriend::~CModuleFriend()
{
}

void CModuleFriend::Final()
{
	m_pGiftVersion->Clear();
	SAFE_DELETE(m_pGiftVersion);
	for (auto iter : mapFriendList)
		SAFE_DELETE(iter.second);
	mapFriendList.clear();
	CModule::Final();
}

bool CModuleFriend::Init(CGameUser* pUser)
{
	if (!CModule::Init(pUser))
		return false;
	return true;
}

void CModuleFriend::MsgRequestFriend(uint64 llRoleId)
{
}

void CModuleFriend::MsgAgreeFriend(uint64 llRoleId)
{
}

void CModuleFriend::MsgRefuseFriend(uint64 llRoleId)
{
}

void CModuleFriend::MsgAgreeAll()
{
}

void CModuleFriend::MsgRefuseAll()
{
}

void CModuleFriend::MsgRemoveFriend(uint64 llRoleId)
{
}

void CModuleFriend::MsgBlockRole(uint64 llRoleId)
{
}

void CModuleFriend::MsgSearchRole(const std::string& strNick)
{
}

void CModuleFriend::MsgSetFilter(bool bCity, uint32 uiSex, uint32 uiBitLabel)
{
}

void CModuleFriend::MsgGetRecommendList()
{
}

void CModuleFriend::MsgSetMyLabel(uint32 uiCity, bool bAvailable, uint32 uiBitLabel)
{
}

void CModuleFriend::MsgSendGift(uint64 llRecver, uint32 uiItemUid, uint64 llItemCount)
{
}

void CModuleFriend::MsgReceiveGift(uint64 llGiftUid)
{
}

bool CModuleFriend::UseExpandItem(uint32 uiItemTid, bool bBind, uint32 uiAddValue)
{
	return true;
}

bool CModuleFriend::IsMyFriend(uint32 uiRoleId)
{
	return true;
}