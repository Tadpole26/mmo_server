#include "innerteam.pb.h"
#include "teamcommon.pb.h"
#include "team.pb.h"

#include "TeamUser.h"
#include "TeamMember.h"
#include "ParamConfig.h"

TeamMember::TeamMember()
{
}

TeamMember::~TeamMember()
{
}

void TeamMember::final()
{
}

bool TeamMember::isLeader()
{
	assert(_team != nullptr);
	//return _team->isLeader(this);
}

bool TeamMember::isExpireReplace()
{
	return _replace <= GetCurrTime();
}

bool TeamMember::isExpireInvite()
{
	return _invite <= GetCurrTime();
}

bool TeamMember::isExpireTogether()
{
	return _together <= GetCurrTime();
}

bool TeamMember::isExpireAppoint()
{
	return _appoint <= GetCurrTime();
}

void TeamMember::setIndex(uint32 index)
{
	_index = index;
}

void TeamMember::setOnline()
{
	_state = true;
}

void TeamMember::setFollow()
{
	_follow = true;
}

void TeamMember::setExpireReplace()
{
	_replace = GetCurrTime() + gParamCfg->teamReplaceColdSec;
}

void TeamMember::setExpireInvite()
{
	_invite = GetCurrTime() + gParamCfg->teamInviteColdSec;
}

void TeamMember::setExpireTogether()
{
	_together = GetCurrTime() + gParamCfg->teamTogetherColdSec;
}

void TeamMember::setExpireAppoint()
{
	_appoint = GetCurrTime() + gParamCfg->teamApplyColdSec;
}

void TeamMember::resetOnline()
{
	_state = false;
}

void TeamMember::resetFollow()
{
	_follow = false;
}

void TeamMember::resetExpireReplace()
{
	_replace = 0;
}

void TeamMember::resetExpireInvite()
{
	_invite = 0;
}

void TeamMember::resetExpireTogether()
{
	_together = 0;
}

void TeamMember::resetExpireAppoint()
{
	_appoint = 0;
}

void TeamMember::tryExpireRemove()
{
	if (isExpireReplace()) resetExpireReplace();
	if (isExpireInvite()) resetExpireInvite();
	if (isExpireTogether()) resetExpireTogether();
}

void TeamMember::jumpToLeader(uint32 eJumpTo, uint32 sceneHashId)
{
}

void TeamMember::fill(common::PlatTeamMember& out)
{
	out.set_careerid(pUser->getCareerId());
	out.set_level(pUser->getLevel());
}

void TeamMember::fill(common::TeamMember& out)
{
	pUser->fill(*out.mutable_role());
	pUser->fill(*out.mutable_extd1());
	pUser->fill(*out.mutable_extd2());
	pUser->fill(*out.mutable_extd3());
	out.set_index(getIndex());
	out.set_state(getState());
	out.set_follow(getFollow());
}

void TeamMember::fill(common::RecruitTeamMember& out)
{
	out.set_headicon(pUser->getHeadIcon());
	out.set_headframe(pUser->getHeadFrame());
	out.set_careerid(pUser->getCareerId());
	out.set_level(pUser->getLevel());
	out.set_sex(pUser->getSex());
}

void TeamMember::fillShare(common::TeamMemberFieldList& out, uint32 fieldId)
{
	switch (fieldId)
	{
	case common::TMFieldType_Index: { auto& one = *out.add_tfid();						one.set_type(fieldId); one.set_val(getIndex()); } break;
	case common::TMFieldType_State: { auto& one = *out.add_tfbool();					one.set_type(fieldId); one.set_val(getState()); } break;
	case common::TMFieldType_Follow: { auto& one = *out.add_tfbool();					one.set_type(fieldId); one.set_val(getFollow()); } break;
	case common::TMFieldType_NickName: { auto& one = *out.add_tfbyte();					one.set_type(fieldId); one.set_val(pUser->getNickName()); } break;
	case common::TMFieldType_HeadIcon: { auto& one = *out.add_tfid();					one.set_type(fieldId); one.set_val(pUser->getHeadIcon()); } break;
	case common::TMFieldType_HeadFrame: { auto& one = *out.add_tfid();					one.set_type(fieldId); one.set_val(pUser->getHeadFrame()); } break;
	case common::TMFieldType_CareerId: { auto& one = *out.add_tfid();					one.set_type(fieldId); one.set_val(pUser->getCareerId()); } break;
	case common::TMFieldType_Level: { auto& one = *out.add_tfid();						one.set_type(fieldId); one.set_val(pUser->getLevel()); } break;
	case common::TMFieldType_MapInfo: { auto& one = *out.add_tfbyte();					one.set_type(fieldId); one.set_val(pUser->getMapInfoByte()); } break;
	case common::TMFieldType_MapPosition: { auto& one = *out.add_tfbyte();				one.set_type(fieldId); one.set_val(pUser->getMapPosByte()); } break;
	case common::TMFieldType_Sex: { auto& one = *out.add_tfid();						one.set_type(fieldId); one.set_val(pUser->getSex()); } break;
	case common::TMFieldType_Model: { auto& one = *out.add_tfbyte();					one.set_type(fieldId); one.set_val(pUser->getModelByte()); } break;
	case common::TMFieldType_FightPoint: { auto& one = *out.add_tfid();					one.set_type(fieldId); one.set_val(pUser->getFightPoint()); } break;
	case common::TMFieldType_Hp: { auto& one = *out.add_tfid();							one.set_type(fieldId); one.set_val(pUser->getHp()); } break;
	case common::TMFieldType_MaxHp: { auto& one = *out.add_tfid();						one.set_type(fieldId); one.set_val(pUser->getMaxHp()); } break;
	case common::TMFieldType_Mp: { auto& one = *out.add_tfid();							one.set_type(fieldId); one.set_val(pUser->getMp()); } break;
	case common::TMFieldType_MaxMp: { auto& one = *out.add_tfid();						one.set_type(fieldId); one.set_val(pUser->getMaxMp()); } break;
	case common::TMFieldType_CareerLevel: { auto& one = *out.add_tfid();				one.set_type(fieldId); one.set_val(pUser->getCareerLevel()); } break;
	case common::TMFieldType_Fashion: { auto& one = *out.add_tfbyte();					one.set_type(fieldId); one.set_val(pUser->getFashionByte()); } break;
	case common::TMFieldType_MainHand: { auto& one = *out.add_tfbyte();					one.set_type(fieldId); one.set_val(pUser->getMainHandByte()); } break;
	case common::TMFieldType_OffHand: { auto& one = *out.add_tfbyte();					one.set_type(fieldId); one.set_val(pUser->getOffHandByte()); } break;
	defaut: { assert(false); }  break;
	}
}

void TeamMember::fillSelf(common::TeamMemberFieldList& out, uint32 fieldId)
{
	switch (fieldId)
	{
	case common::TMFieldType_Replace: { auto& one = *out.add_tfid(); one.set_type(fieldId); one.set_val(getExpireReplace()); } break;
	case common::TMFieldType_Invite: { auto& one = *out.add_tfid(); one.set_type(fieldId); one.set_val(getExpireInvite()); } break;
	case common::TMFieldType_Together: { auto& one = *out.add_tfid(); one.set_type(fieldId); one.set_val(getExpireTogether()); } break;
	case common::TMFieldType_Appoint: { auto& one = *out.add_tfid(); one.set_type(fieldId); one.set_val(getExpireAppoint()); } break;
	defaut: { assert(false); }  break;
	}
}

void TeamMember::fill(inner::InnerTeamMember& out)
{
	out.set_index(getIndex());
	out.set_state(getState());
	out.set_follow(getFollow());
	out.set_sceneid(pUser->getSceneId());
}

void TeamMember::fill(inner::InnerTeamMemberField& out, uint32 fieldId)
{
	switch (fieldId)
	{
	case inner::TMFType_Index: { out.set_index(getIndex()); } break;
	case inner::TMFType_State: { out.set_state(getState()); } break;
	case inner::TMFType_Follow: { out.set_follow(getFollow); } break;
	case inner::TMFType_SceneId: { out.set_sceneid(pUser->getSceneId()); } break;
	defaut: { assert(false); } break;
	}
}

void TeamMember::__setTeam(SessionTeam* team)
{
	assert(team != nullptr);
	assert(_team == nullptr);
	_team = team;
}

void TeamMember::__setNotTeam()
{
	assert(_team != nullptr);
	_team = nullptr;
}

bool  TeamMember::sendChgFieldToMe(std::initializer_list<uint32> changes)
{
	client::ModuleTeam_Ntf_UpdateMember ntf;
	ntf.set_roleid(pUser->getRoleId());
	for (auto fieldId : changes)
	{
		fillSelf(*ntf.mutable_fieldlist(), fieldId);
	}
	//return sendCmdToMe(client::enClientFirst_Team, client::enSecondTeam_Ntf_UpdateMember, ntf);
}

bool  TeamMember::sendChgFieldToTeam(std::initializer_list<uint32> changes)
{
	client::ModuleTeam_Ntf_UpdateMember ntf;
	ntf.set_roleid(pUser->getRoleId());
	for (auto fieldId : changes)
	{
		fillShare(*ntf.mutable_fieldlist(), fieldId);
	}
	//return teamRef().sendCmdToTeam(client::enClientFirst_Team, client::enSecondTeam_Ntf_UpdateMember, ntf);
}

bool  TeamMember::broadChgFieldToScene(std::initializer_list<uint32> changes)
{
	/*
	return NetInterface::get().broadCmdSceneSvrAuto([&](auto &out)
	{
		auto &ntf = *out.mutable_fromteam_memeberfield();
		ntf.set_teamid(teamRef().getTeamId());
		ntf.set_roleid(pUser->getRoleId());
		for (auto fieldId : changes)
		{
			fill(*ntf.add_fields(), fieldId);
		}
	})
	*/
}