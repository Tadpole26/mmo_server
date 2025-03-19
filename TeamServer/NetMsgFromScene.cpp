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
	}
	break;
	case inner::InnerTeamsvr::FromsceneCase::kFromsceneLeadercancelmatch:
	{
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