#pragma once
#include "CModule.h"

namespace record
{
	class RecordModuleFriend;
	class RecordSendGiftVersion;
}

class CModuleFriend : public CModule
{
public:
	//好友关系
	struct friend_relation_t
	{
		uint64 ullFrdId = 0;						//好有id
		uint32 uiIntimacy = 0;						//亲密度
	};
	CModuleFriend();
	virtual ~CModuleFriend();
	virtual void Final() final;
	virtual bool Init(CGameUser* pUser) final;
public:
	void MsgRequestFriend(uint64 llRoleId);
	void MsgAgreeFriend(uint64 llRoleId);
	void MsgRefuseFriend(uint64 llRoleId);
	void MsgAgreeAll();
	void MsgRefuseAll();
	void MsgRemoveFriend(uint64 llRoleId);
	void MsgBlockRole(uint64 llRoleId);
	void MsgSearchRole(const std::string& strNick);
	void MsgSetFilter(bool bCity, uint32 uiSex, uint32 uiBitLabel);
	void MsgGetRecommendList();
	void MsgSetMyLabel(uint32 uiCity, bool bAvailable, uint32 uiBitLabel);
	void MsgSendGift(uint64 llRecver, uint32 uiItemUid, uint64 llItemCount);
	void MsgReceiveGift(uint64 llGiftUid);
	bool UseExpandItem(uint32 uiItemTid, bool bBind, uint32 uiAddValue);
	bool IsMyFriend(uint32 uiRoleId);
private:
	record::RecordModuleFriend* m_pGiftVersion = nullptr;
	std::unordered_map<uint64, friend_relation_t*> mapFriendList;			//好友列表
};