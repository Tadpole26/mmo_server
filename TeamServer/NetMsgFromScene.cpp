#include "parse_pb.h"
#include "teamsvr.pb.h"

#include "TeamUser.h"
#include "TeamUserMgr.h"
#include "NetMsgFromScene.h"

NetMsgFromScene::NetMsgFromScene()
{
}

NetMsgFromScene::~NetMsgFromScene()
{
}

void NetMsgFromScene::trigger()
{
	//Msg_ServerCommon_BeatHart_Req oBeatReq;
	//Send_Msg(&oBeatReq, MsgModule_ServerCommon::Msg_ServerCommon_BeatHart_Req, MsgModule::ServerCommon);
}

void NetMsgFromScene::handle_msg(const tagMsgHead* pNetMsg)
{
	if (nullptr == pNetMsg) return;
	inner::InnerTeamsvr innerReq;
	PARSE_PTL_HEAD(innerReq, pNetMsg);

	TeamUser* pUser = nullptr;
	if (innerReq.fromzone() && innerReq.fromuser())
	{
		pUser = gTeamUserMgr->tryCreateRole(innerReq.fromzone(), innerReq.fromuser());
		if (pUser == nullptr)
		{
			Log_Error("tryCreateRole.%s,%s", innerReq.GetTypeName().c_str(), innerReq.ShortDebugString().c_str());
		}
	}
	switch (innerReq.Fromscene_case())
	{
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneGameevent:
	{
		const auto& req = innerReq.fromscene_gameevent();
		switch (req.gameevent().type_case())
		{
		case inner::InnerGameEvent::kDestroy: break;
		case inner::InnerGameEvent::kLogin: break;
		case inner::InnerGameEvent::kLogout: break;
		case inner::InnerGameEvent::kOnline: break;
		case inner::InnerGameEvent::kOffline: break;
		case inner::InnerGameEvent::kDeath: break;
		case inner::InnerGameEvent::kRebirth: break;
		case inner::InnerGameEvent::kWakeup: break;
		case inner::InnerGameEvent::kChangemap: break;
		}
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneUpdateuserinfo:
	{
		const auto& req = innerReq.fromscene_updateuserinfo();
		switch (req.type_case())
		{
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kExtd2: break;
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kExtd3: break;
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kNickName:break;
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kHeadIcon:break;
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kHeadFrame:break;
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kCareerId:break;
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kLevel:break;
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kSex:break;
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kCareeLevel:break;
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kF33:break;
		case inner::InnerTeamsvr_Fromscene_UpdateUserInfo::kF55:break;
		}
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromscenePersonchangetarget:
	{
		const auto& req = innerReq.fromscene_personchangetarget();
		pUser->innerPersonChangeTarget(req.targetid());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromscenePersoncancelmatch:
	{
		const auto& req = innerReq.fromscene_personcancelmatch();
		pUser->innerPersonCancelMatch();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneLeaderstartmatch:
	{
		const auto& req = innerReq.fromscene_leaderstartmatch();
		pUser->innerLeaderStartMatch(req.text(), req.languageid());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneLeadercancelmatch:
	{
		const auto& req = innerReq.fromscene_leadercancelmatch();
		pUser->innerLeaderCancelMatch();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneTargetteamlist:
	{
		//tr队伍列表拉去走http
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneCreateteam:
	{
		const auto& req = innerReq.fromscene_createteam();
		pUser->innerCreateTeam(req.targetid(), req.minlevel(), req.maxlevel());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneFiremember:
	{
		const auto& req = innerReq.fromscene_firemember();
		pUser->innerLeaderFireMember(req.roleid());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneLeaderchangetarget:
	{
		const auto& req = innerReq.fromscene_leaderchangetarget();
		pUser->innerLeaderChangeTarget(req.targetid(), req.minlevel(), req.maxlevel());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneLeadertogethermember:
	{
		const auto& req = innerReq.fromscene_leadertogethermember();
		pUser->innerLeaderTogetherMember(req.roleid());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneLeaderappoint:
	{
		const auto& req = innerReq.fromscene_leaderappoint();
		pUser->innerleaderAppoint(req.roleid());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneLeaderrefuseallapply:
	{
		const auto& req = innerReq.fromscene_leaderrefuseallapply();
		pUser->innerLeaderRefuseAllApply();
	}
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneLeaderagreeallapply:
	{
		const auto& req = innerReq.fromscene_leaderagreeallapply();
		pUser->innerLeaderAgreeAllApply();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneLeaderrefuseapply:
	{
		const auto& req = innerReq.fromscene_leaderrefuseapply();
		pUser->innerLeaderRefuseApply(req.zoneidf(), req.roleid());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneLeaderagreeapply:
	{
		const auto& req = innerReq.fromscene_leaderagreeapply();
		pUser->innerLeaderAgreeApply(req.zoneidf(), req.roleid());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneInvitefriend:
	{
		const auto& req = innerReq.fromscene_invitefriend();
		pUser->innerInviteFriend(req.roleid());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneInvitefamily:
	{
		const auto& req = innerReq.fromscene_invitefamily();
		pUser->innerInviteFamily(req.roleid());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneInviteplayer:
	{
		const auto& req = innerReq.fromscene_inviteplayer();
		pUser->innerInvitePlayer(req.zoneidf(), req.roleid());
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneAgreeinvited:
	{
		const auto& req = innerReq.fromscene_agreeinvited();
		pUser->innerAgreeInvited(1);
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneRefuseinvited:
	{
		const auto& req = innerReq.fromscene_refuseinvited();
		pUser->innerRefuseInvited();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneBatleaveteam:
	{
		const auto& req = innerReq.fromscene_batleaveteam();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneApplyteam:
	{
		const auto& req = innerReq.fromscene_applyteam();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneFollowleader:
	{
		const auto& req = innerReq.fromscene_followleader();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneCancelfollow:
	{
		const auto& req = innerReq.fromscene_cancelfollow();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneAgreetogether:
	{
		const auto& req = innerReq.fromscene_agreetogether();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneRefusetogether:
	{
		const auto& req = innerReq.fromscene_refusetogether();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneReplaceleader:
	{
		const auto& req = innerReq.fromscene_replaceleader();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromscenePersonvote:
	{
		const auto& req = innerReq.fromscene_personvote();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneEnterbyvote:
	{
		const auto& req = innerReq.fromscene_enterbyvote();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneChatteam:
	{
		const auto& req = innerReq.fromscene_chatteam();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneBatcreateteam:
	{
		const auto& req = innerReq.fromscene_batcreateteam();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneBatdestroyteam:
	{
		const auto& req = innerReq.fromscene_batdestroyteam();
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneJumptoleader:
	{
		const auto& req = innerReq.fromscene_jumptoleader();
	}
	break;
	default:
		Log_Error("Unknown from scene case!", innerReq.Fromscene_case());
		break;
	}
	
	
}

void NetMsgFromScene::on_connect()
{
	/*
	Msg_ServerCommon_Register_Req oRegisterReq;
	oRegisterReq.set_uiserverid(GetServerID());
	oRegisterReq.set_uiserverkind(SERVER_KIND_GAME);
	oRegisterReq.set_uiplatid(GAME_LOGIC_INS->m_oConstCfg.m_uiPlatId);
	oRegisterReq.set_uigroupid(GAME_LOGIC_INS->m_oConstCfg.m_uiGroupId);
	Send_Msg(&oRegisterReq, MsgModule_ServerCommon::Msg_ServerCommon_Register_Req, MsgModule::ServerCommon);
	*/
}

void NetMsgFromScene::on_disconnect()
{
	Log_Warning("scene server discoonnect!");
}