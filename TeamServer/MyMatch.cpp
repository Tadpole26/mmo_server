#include "match.pb.h"

#include "ParamConfig.h"

#include "cMatch.h"

#include "TeamMember.h"
#include "TeamUser.h"
#include "TeamLeader.h"
#include "SessionTeam.h"
#include "MatchUnitMgr.h"
#include "MatchRoomMgr.h"

void TeamUser::mountMatch()
{
	assert(pUnit != nullptr);
	Log_Debug("mountMatch");
	client::ModuleMatch_Ntf_MountMatch ntf;
	pUnit->fill(*ntf.mutable_mstatus());
	if (pUnit->room != nullptr)
	{
		pUnit->room->fill(*ntf.mutable_grouplist());
	}
	//sendCmdToMe(client::enClientFirst_Match, client::enSecondMatch_Ntf_MountMatch, ntf);
}

void TeamUser::forceCancelToMatch()
{
	/*
		NetInterface::get().sendCmdMatchsvr([&](auto& out) 
		{
			out.set_fromuser(getRoleId());
			auto& ntf = *out.mutable_fromteam_forcecamcel();
		});
	*/
}

bool TeamUser::isPunishExpire()
{
	return _punishExpire <= GetCurrTime();
}

uint32 TeamUser::getPunishExpire()
{
	return _punishExpire;
}

void TeamUser::setMatchPunish(uint32 ePunish)
{
	_punishExpire = isPunishExpire() ? GetCurrTime() : getPunishExpire();
	switch (ePunish)
	{
	case cMatch::ePunishEscape: _punishExpire += gParamCfg->escaprePunishSec; break;
	case cMatch::ePunishRefuse: _punishExpire += gParamCfg->refusePunishSec; break;
	case cMatch::ePunishReset: _punishExpire = GetCurrTime(); break;
	}
	Log_Debug("setPunishExpire.%u,%lu", ePunish, _punishExpire - GetCurrTime());
}

bool TeamUser::innerMatchStartMatch(const inner::InnterCopyType& in, uint32 mtype)
{
	/*
	auto funcNotTeam = ([&]()->bool 
	{
			if (pUnit != nullptr)
			{
				Log_Error("innerMatchStartMatch.funcNotTeam");
				return false;
			}
			/*if (isInCopyScene() && !isInCopyScene(mtype))
			{
				Log_Error("innerMatchStartMatch.funcNotTeam");
				return false;
			}
			if (!isPunishExpire())
			{
				return false;
			}

			if (isTeamPersonMatching())
			{
				Log_Error("innerMatchStartMatch.funcNotTeam");
				return false;
			}
			if (isVoting())
			{
				Log_Error("innerMatchStartMatch.funcNotTeam");
				return false;
			}
			Log_Info("innerMatchStartMatch.单人.无队");
			/*
			return NetInterface::get().sendCmdMatchsvr([&](auto& out) 
			{
				auto& ntf = *out.mutable_fromteam_startamtch();
				ntf.set_mtype(mtype);
				*ntf.mutable_copy() = in;
				* auto& roles = *ntf.mutable_roles();
				* fill(mtype, roles[getRoleId()]);
			});
			
	});
	auto funcHasTeam = ([&](SessionTeam& team)->bool 
	{
		if (!pMem->isLeader())
		{
			Log_Error("innerMatchStartMatch");
			return false;
		}
		if (team.isMatching())
		{
			Log_Error("innerMatchStartMatch");
			return false;
		}
		if (team.isVoting())
		{
			Log_Error("innerMatchStartMatch");
			return false;
		}
		if (!team.foreachMem([](auto& memRef)->bool
			{
				return memRef.pUser->pUnit == nullptr;
			}))
		{
			Log_Error("innerMatchStartMatch");
			//team.foreachMem([&](auto& memRef) { return true;});
			return false;
		}
		if (!team.foreachMem([&](auto& memRef)->bool
		{
			//if (memRef.pUser->isInStaticScene())
			//{
			//	return true;
			//}
			//if (!memRef.pUser->isInCopyScene(mtype))
			//{
			//	return true;
			//}
			if (team.teamLeaderRef().getSceneHashId() != memRef.pUser->getSceneHashId())
			{
				return true;
			}
			if (team.teamLeaderRef().getSceneId() != memRef.pUser->getSceneId())
			{
				return true;
			}
			return true;			
		}))
		{
			Log_Error("innerMatchStartMatch");
			//team.foreachMem([&](auto& memRef) { return true;});
			return false;
		}
		TeamUser* pUserPunish = nullptr;
		team.foreachMem([&](auto& memRef) 
		{
			if (memRef.pUser->isPunishExpire())
			{
				return true;
			}
			if (pUserPunish == nullptr || memRef.pUser->getPunishExpire() > pUserPunish->getPunishExpire())
			{
				pUserPunish = memRef.pUser;
			}
			return true;
		});
		if (pUserPunish)
		{
			Log_Error("innerMatchStartMatch");
			auto expire = pUserPunish->getPunishExpire() - GetCurrTime();
			team.foreachMem([&](auto& memRef) { return true; });
			return true;
		}
		if (!team.foreachMem([](auto& memRef)->bool
			{
				return memRef.isOnline();
			}))
		{
			Log_Error("innerMatchStartMatch");
			team.foreachMem([&](auto& memRef) { return true; });
			return false;
		}
		return true;
	});
	auto funcHasTeamForF55 = ([&](SessionTeam& team)->bool 
	{
			//进行各种判断
			/*
			*  NetInterface::get().sendCmdMatchsvr([&](auto& out) 
			{
				auto& ntf = *out.mutable_fromteam_startmatch();
				ntf.set_mtype(mtype);
				*ntf.mutable_copy() = in;
				auto& roles = *ntf.mutable_roles();
				pMem->teamRef().foreachMem([&](auto& memRef) 
				{
					memRef.pUser->fill(mtype, roles[memRef.pUser->getRoleId()]);
					return true;
				});
			});
			
	});
	if (pMem == nullptr)
	{
		return funcNotTeam();
	}
	else if (funcHasTeam(pMem->teamRef()))
	{
		switch (mtype)
		{
		case cMatch::eMatchF55: return funcHasTeamForF55(pMem->teamRef()); break;
		}
		Log_Error("innerMatchStartMatch");
		return false;
	}
	else
	{
		Log_Error("innerMatchStartMatch");
		return false;
	}
	*/
		return true;
}

bool TeamUser::innerMatchCancelMatch()
{
	if (pUnit == nullptr)
	{
		Log_Error("innerMatchCancelMatch");
		return false;
	}
	if (pUnit->room != nullptr)
	{
		Log_Error("innerMatchCancelMatch");
		return false;
	}
	/*
	return NetInterface::get().sendCmdMatchsvr([&](auto& out) 
	{
		out.set_fromuser(getRoleId());
		auto& ntf = *out.mutable_fromteam_cancelmatch();
		ntf.set_unitid(pUnit->getUnitId);
	});
	*/
}

bool TeamUser::innerMatchConfirmOk()
{
	if (pUnit == nullptr)
	{
		Log_Error("innerMatchCancelMatch");
		return false;
	}
	if (pUnit->room == nullptr)
	{
		Log_Error("innerMatchCancelMatch");
		return false;
	}
	if (pUnit->room->isConfirmed(getRoleId()))
	{
		Log_Error("innerMatchCancelMatch");
		return false;
	}
	/*
	return NetInterface::get().sendCmdMatchsvr([&](auto& out) 
	{
		out.set_fromuser(getRoleId());
		auto& ntf = *out.mutable_fromteam_confirmok();
	});
	*/
}

bool TeamUser::innerMatchRefuseOk()
{
	if (pUnit == nullptr)
	{
		Log_Error("innerMatchCancelMatch");
		return false;
	}
	if (pUnit->room == nullptr)
	{
		Log_Error("innerMatchCancelMatch");
		return false;
	}
	if (pUnit->room->isConfirmed(getRoleId()))
	{
		Log_Error("innerMatchCancelMatch");
		return false;
	}
	/*
	return NetInterface::get().sendCmdMatchsvr([&](auto& out)
	{
		out.set_fromuser(getRoleId());
		auto& ntf = *out.mutable_fromteam_fefuseok();
	});
	*/
}

bool TeamUser::innerMatchPunish(uint32 ePunish)
{
	setMatchPunish(ePunish);
	return true;
}

bool TeamUser::innerMatchOpCancel()
{
	client::ModuleMatch_Ntf_CopyMatchOpCancel ntf;
	if (pMem == nullptr)
	{
		//return sendCmdToMe(client::enClientFirst_Match, client::enSecondMatch_Ntf_CopyMatchOperaionCancel, ntf);
	}
	return pMem->teamRef().foreachMem([&](auto& memRef)->bool
	{
			auto* pMemUser = memRef.pUser;
			//pMemUser->sendCmdToMe(client::enClientFirst_Match, client::enSecondMatch_Ntf_CopyMatchOperaionCancel, ntf);
			return true;
	});
}

void TeamUser::openMatchPanelNtf()
{
	assert(pUnit != nullptr);
	/*
	client::SecondMatch_Ntf_OpenMatchPanel ntf;
	ntf.set_type(pUnit->getMatchType());
	ntf.set_matchtime(pUnit->getMatchTime());
	sendCmdToMe(client::enClientFirst_Match, client::enSecondMatch_Ntf_OpenMatchPanel, ntf);
	*/
}

void TeamUser::closeMatchPanelNtf()
{
	assert(pUnit != nullptr);
	/*
	client::SecondMatch_Ntf_CloseMatchPanel ntf;
	sendCmdToMe(client::enClientFirst_Match, client::enSecondMatch_Ntf_CloseMatchPanel, ntf);
	*/
}

void TeamUser::closeConfirmPanelNtf()
{
	assert(pUnit != nullptr);
	/*
	client::SecondMatch_Ntf_CloseConfirmPanel ntf;
	sendCmdToMe(client::enClientFirst_Match, client::enSecondMatch_Ntf_CloseConfirmPanel, ntf);
	*/
}