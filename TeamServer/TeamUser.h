#pragma once
#include "sdk.h"
#include "cUser.h"

namespace inner
{
	class InnerGameEvent;
}

namespace common
{
	class RoleBrief;
	class RoleExtend1;
	class RoleExtend2;
	class RoleExtend3;
	class ExtendFight3v3;
	class ExtendFight5v5;
}

class TeamMember;
class TeamUser : public cUser
{
private:
	uint32 _sceneHashId = 0;
	common::RoleBrief* _brief = nullptr;
	common::RoleExtend1* _extd1 = nullptr;
	common::RoleExtend2* _extd2 = nullptr;
	common::RoleExtend3* _extd3 = nullptr;
	common::ExtendFight3v3* _f33 = nullptr;
	common::ExtendFight5v5* _f55 = nullptr;
public:
	TeamMember* pMem = nullptr;						//当为队伍成员时不为空
public:
	void fill(common::RoleBrief& out);
	void fill(common::RoleExtend1& out);
	void fill(common::RoleExtend2& out);
	void fill(common::RoleExtend3& out);
	void fill(common::ExtendFight3v3& out);
	void fill(common::ExtendFight5v5& out);
public:
	const std::string getMapInfoByte() { return _extd2->sceneid().SerializeAsString(); }
	const std::string getMapPosByte() { return _extd2->cpos().SerializeAsString(); }
	const std::string getModelByte() { return _extd1->mplist().SerializeAsString(); }
	const std::string getFashionByte() { return _extd1->mplist().SerializeAsString(); }
	const std::string getMainHandByte() { return _extd1->mainhand().SerializeAsString(); }
	const std::string getOffHandByte() { return _extd1->offhand().SerializeAsString(); }
	uint32 getHeadIcon() { return _brief->headicon(); }
	uint32 getHeadFrame() { return _brief->headframe(); }
	uint32 getCareerId() { return _brief->careerid(); }
	uint32 getLevel() { return _brief->level(); }
	uint32 getCareerLevel() { return _brief->careerid(); }
	uint32 getHp() { return _extd3->hp(); }
	uint32 getMp() { return _extd3->mp(); }
	uint32 getMaxHp() { return _extd3->maxhp(); }
	uint32 getMaxMp() { return _extd3->maxmp(); }
	uint32 getSex() { return _brief->sex(); }
	uint32 getFightPoint() { return _extd1->fightpoint(); }
	uint32 getChatFont() { return _extd1->chatfont(); }
	uint32 getChatFrame() { return _extd1->chatframe(); }
	uint32 getEproc() { }
	uint32 getSceneHashId() { return _sceneHashId; }
	zSceneIdType getSceneId() {}
public:
	void setSceneHashId(uint32 hashId);
	void setBrief(const common::RoleBrief& in);
	void setExtd1(const common::RoleExtend1& in);
	void setExtd2(const common::RoleExtend2& in);
	void setExtd3(const common::RoleExtend3& in);
	void setF33(const common::ExtendFight3v3& in);
	void setF55(const common::ExtendFight5v5& in);
public:
	void evGameDestroy(const inner::InnerGameEvent& in);
	void evGameLogin(const inner::InnerGameEvent& in);
	void evGameLogout(const inner::InnerGameEvent& in);
	void evGameOnline(const inner::InnerGameEvent& in);
	void evGameOffline(const inner::InnerGameEvent& in);
	void evGameDeath(const inner::InnerGameEvent& in);
	void evGameRebirth(const inner::InnerGameEvent& in);
	void evGameWakeup(const inner::InnerGameEvent& in);
	void evGameChangeMap(const inner::InnerGameEvent& in);
public:
	void mountTeam();
};