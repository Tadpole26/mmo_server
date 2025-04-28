#include "parse_pb.h"
#include "teamsvr.pb.h"

#include "TeamUser.h"
#include "TeamUserMgr.h"
#include "MatchUnitMgr.h"
#include "MatchRoomMgr.h"
#include "NetMsgFromMatch.h"

NetMsgFromMatch::NetMsgFromMatch()
{
}

NetMsgFromMatch::~NetMsgFromMatch()
{
}

void NetMsgFromMatch::trigger()
{
	//Msg_ServerCommon_BeatHart_Req oBeatReq;
	//Send_Msg(&oBeatReq, MsgModule_ServerCommon::Msg_ServerCommon_BeatHart_Req, MsgModule::ServerCommon);
}

void NetMsgFromMatch::handle_msg(const tagMsgHead* pNetMsg)
{
	if (nullptr == pNetMsg) return;
	inner::InnerTeamsvr innerReq;
	PARSE_PTL_HEAD(innerReq, pNetMsg);

	switch (innerReq.Frommatch_case())
	{
	case inner::InnerTeamsvr::FrommatchCase::kFrommatchStartmatch:
	{
		const auto& req = innerReq.frommatch_startmatch();
		auto* pUnit = gMatchUnitMgr->createUnit(req.unitid(), req.mtype());
		if (pUnit != nullptr)
		{
			for (auto i = 0; i < req.matchlist().roleids_size(); ++i)
			{
				auto* pUser = gTeamUserMgr->getRole(req.matchlist().roleids(i));
				pUnit->addMember(pUser);
			}
			if (pUnit->emptyMember())
			{
				gMatchUnitMgr->addToRecycle(pUnit);
			}
			else
			{
				pUnit->startMatch();
			}
		}
	}
	break;
	case inner::InnerTeamsvr::FrommatchCase::kFrommatchRestartmatch:
	{
		const auto& req = innerReq.frommatch_restartmatch();
		auto* pUnit = gMatchUnitMgr->getUnit(req.unitid());
		if (pUnit != nullptr)
		{
			switch (req.type())
			{
			case cMatch::eRestartMatchFromRoom: pUnit->restartMatch();					break;
			case cMatch::eRestartMatchFromQueue: pUnit->restartMatchFromQueue();		break;
			default:	assert(false);													break;
			}
		}
	}
	break;
	case inner::InnerTeamsvr::FrommatchCase::kFrommatchCancelmatch:
	{
		const auto& req = innerReq.frommatch_cancelmatch();
		auto* pUnit = gMatchUnitMgr->getUnit(req.unitid());
		if (pUnit != nullptr)
		{
			pUnit->cancelMatch();
		}
	}
	break;
	case inner::InnerTeamsvr::FrommatchCase::kFrommatchPunishmatch:
	{
		const auto& req = innerReq.frommatch_punishmatch();
		auto* pUnit = gMatchUnitMgr->getUnit(req.unitid());
		if (pUnit != nullptr)
		{
			pUnit->punishMatch(req.punishlist());
		}
	}
	break;
	case inner::InnerTeamsvr::FrommatchCase::kFrommatchStartconfirm:
	{
		const auto& req = innerReq.frommatch_startconfirm();
		auto* pRoom = gMatchRoomMgr->createRoom(req.room());
		if (pRoom != nullptr) 
		{
			pRoom->startConfirm(req.expire());
		}
	}
	break;
	case inner::InnerTeamsvr::FrommatchCase::kFrommatchRoleconfirmok:
	{
		const auto& req = innerReq.frommatch_roleconfirmok();
		auto* pRoom = gMatchRoomMgr->getRoom(req.roomid());
		if (pRoom != nullptr)
		{
			pRoom->setConfirmOk(innerReq.fromuser());
		}
	}
	break;
	case inner::InnerTeamsvr::FrommatchCase::kFrommatchStartloading:
	{
		const auto& req = innerReq.frommatch_startloading();
		auto* pRoom = gMatchRoomMgr->getRoom(req.roomid());
		if (pRoom != nullptr)
		{
			pRoom->setCopyScene(req.copyscene());
			pRoom->setCreateBy(req.createby());
			pRoom->startLoading();
		}
	}
	break;
	default:
		Log_Error("Unknown from match case!", innerReq.Frommatch_case());
		break;
	}


}

void NetMsgFromMatch::on_connect()
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

void NetMsgFromMatch::on_disconnect()
{
	Log_Warning("match server discoonnect!");
}