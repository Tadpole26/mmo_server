#pragma once
#include "GameModule.h"

namespace record
{
	class RecordModuleFriend;
	class RecordSendGiftVersion;
}

class ModuleFriend : public GameModule
{
public:
	//好友关系
	struct friend_relation_t
	{
		uint64 ullFrdId = 0;						//好有id
		uint32 uiIntimacy = 0;						//亲密度
	};
	ModuleFriend();
	virtual ~ModuleFriend();
	virtual void final() final;
	virtual bool init(GameUser* pUser) final;
public:
	void msgRequestFriend(uint64 llRoleId);
	void msgAgreeFriend(uint64 llRoleId);
	void msgRefuseFriend(uint64 llRoleId);
	void msgAgreeAll();
	void msgRefuseAll();
	void msgRemoveFriend(uint64 llRoleId);
	void msgBlockRole(uint64 llRoleId);
	void msgSearchRole(const std::string& strNick);
	void msgSetFilter(bool bCity, uint32 uiSex, uint32 uiBitLabel);
	void msgGetRecommendList();
	void msgSetMyLabel(uint32 uiCity, bool bAvailable, uint32 uiBitLabel);
	void msgSendGift(uint64 llRecver, uint32 uiItemUid, uint64 llItemCount);
	void msgReceiveGift(uint64 llGiftUid);
	bool useExpandItem(uint32 uiItemTid, bool bBind, uint32 uiAddValue);
	bool isMyFriend(uint32 uiRoleId);
private:
	record::RecordModuleFriend* m_pGiftVersion = nullptr;
	std::unordered_map<uint64, friend_relation_t*> mapFriendList;			//好友列表
};