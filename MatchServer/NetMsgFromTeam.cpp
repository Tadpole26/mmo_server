#include "parse_pb.h"
#include "matchsvr.pb.h"

#include "cMatch.h"

#include "CopyConfig.h"

#include "MatchMgr.h"
#include "MatchQueueMgr.h"
#include "QueueMgrF55.h"
#include "MatchUnit.h"
#include "MatchUnitMgr.h"
#include "MatchMem.h"
#include "MatchMemMgr.h"
#include "MatchRoom.h"
#include "NetMsgFromTeam.h"

NetMsgFromTeam::NetMsgFromTeam()
{
}

NetMsgFromTeam::~NetMsgFromTeam()
{
}

void NetMsgFromTeam::trigger()
{
	//Msg_ServerCommon_BeatHart_Req oBeatReq;
	//Send_Msg(&oBeatReq, MsgModule_ServerCommon::Msg_ServerCommon_BeatHart_Req, MsgModule::ServerCommon);
}

void NetMsgFromTeam::handle_msg(const tagMsgHead* pNetMsg)
{
	if (nullptr == pNetMsg) return;
	inner::InnerMatchsvr innerReq;
	PARSE_PTL_HEAD(innerReq, pNetMsg);
	const auto& req = innerReq.fromteam_startmatch();
	auto eProcThree = req.copy().eproc();
	switch (innerReq.Fromteam_case())
	{
	case inner::InnerMatchsvr::FromteamCase::kFromteamStartmatch:
	{
		Log_Info("fromteam_startmatch.%s", req.ShortDebugString().c_str());
		auto funcF55 = ([&]()->QueueMgrF55* 
		{
			const auto& f55 = req.copy().pvp().f55();
			const auto* cfgcopy = gCopyCfg->getCopyConfig(f55.copyid());
			if (cfgcopy == nullptr)
			{
				Log_Error("fromteam_startmatch.funcF55.!cfgcopy");
				return nullptr;
			}
			return gMatchMgr->f55Ptr(eProcThree, 1, f55.copyid());			//通过发过来进程类型和进程参数传入,TODO
		});
		MatchQueueMgr* pMgr = nullptr;
		switch (req.mtype())
		{
		case cMatch::eMatchF55: pMgr = funcF55();		break;
		default:										break;
		}
		if (pMgr == nullptr)
		{
			Log_Error("fromteam_starmatch.!pMgr");
			return;
		}
		auto* pUnit = gMatchUnitMgr->createUnit(1, *pMgr);					//通过发过来进程类型和进程参数传入,TODO
		if (pUnit == nullptr)
		{
			Log_Error("fromteam_starmatch,createUnitErr");
			return;
		}
		for (auto& p : req.roles())
		{
			auto* pRole = gMatchMemMgr->createRole(*pMgr, p.first, p.second);
			if (pRole == nullptr)
			{
				Log_Error("fromteam_starmatch,createMemberErr");
				pUnit->finalMe();
				return;
			}
			pUnit->addMemRole(pRole);
		}
		pUnit->startMatch();
		return;
	}
	break;
	case inner::InnerMatchsvr::FromteamCase::kFromteamCancelmatch:
	{
		const auto& req = innerReq.fromteam_cancelmatch();
		Log_Info("fromteam_cancelmatch.%s", req.ShortDebugString().c_str());
		auto* pUnit = gMatchUnitMgr->getUnit(req.unitid());
		if (pUnit == nullptr)
		{
			Log_Error("fromteam_cancelmatch,NotMatching");
			return;
		}
		if (pUnit->room != nullptr)
		{
			Log_Error("fromteam_cancelmatch,CanNotCancelMatch");
			return;
		}
		pUnit->cancelMatch();
		return;
	}
	break;
	case inner::InnerMatchsvr::FromteamCase::kFromteamConfirmok:
	{
		const auto& req = innerReq.fromteam_confirmok();
		Log_Info("fromteam_confirmok.%s", req.ShortDebugString().c_str());
		auto* pMem = gMatchMemMgr->getRole(innerReq.fromuser());
		if (pMem == nullptr)
		{
			Log_Error("fromteam_confirmok,NotMatching");
			return;
		}
		if (pMem->isConfirmed())
		{
			Log_Error("fromteam_confirmok,RepeatedConfirm");
			return;
		}
		if (!pMem->canConfirm())
		{
			Log_Error("fromteam_confirmok,CanNotConfirm");
			return;
		}
		pMem->setConfirmOk();
		return;
	}
	break;
	case inner::InnerMatchsvr::FromteamCase::kFromteamRefuseok:
	{
		const auto& req = innerReq.fromteam_refuseok();
		Log_Info("fromteam_refuseok.%s", req.ShortDebugString().c_str());
		auto* pMem = gMatchMemMgr->getRole(innerReq.fromuser());
		if (pMem == nullptr)
		{
			Log_Error("fromteam_refuseok,NotMatching");
			return;
		}
		if (pMem->isConfirmed())
		{
			Log_Error("fromteam_confirmok,RepeatedConfirm");
			return;
		}
		if (!pMem->canConfirm())
		{
			Log_Error("fromteam_confirmok,CanNotConfirm");
			return;
		}
		pMem->setConfirmRefuse();
		return;
	}
	break;
	case inner::InnerMatchsvr::FromteamCase::kFromteamForcecancel:
	{
		const auto& req = innerReq.fromteam_forcecancel();
		Log_Info("fromteam_forcecancel.%s", req.ShortDebugString().c_str());
		auto* pMem = gMatchMemMgr->getRole(innerReq.fromuser());
		if (pMem != nullptr)
		{
			assert(pMem->unit != nullptr);
			auto* pUnit = pMem->unit;
			auto* pRoom = pUnit->room;
			if (pRoom == nullptr)
			{
				pUnit->cancelMatch();
			}
			else
			{
				pMem->setConfirmForceCancel();
				pRoom->forceBreakNtf();
			}
		}
	}
	break;
	default:
		Log_Error("Unknown from team case!", innerReq.Fromteam_case());
		break;
	}
	
	
}

void NetMsgFromTeam::on_connect()
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

void NetMsgFromTeam::on_disconnect()
{
	Log_Warning("team server discoonnect!");
}