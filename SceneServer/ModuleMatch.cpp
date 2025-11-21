#include "client.pb.h"
#include "innercommon.pb.h"

#include "cTeam.h"
#include "cMatch.h"
#include "CopyConfig.h"
#include "MatchConfig.h"

#include "ModuleMatch.h"

ModuleMatch::ModuleMatch() : GameModule(client::enModule_Friend)
{
}

ModuleMatch::~ModuleMatch()
{
}

void ModuleMatch::final()
{
}

bool ModuleMatch::msgStartMatch(uint32 type)
{
	return enterPvpByVote(type);
}

bool ModuleMatch::msgCancelMatch()
{
	return true;
}

bool ModuleMatch::msgConfirmOk()
{
	return true;
}

bool ModuleMatch::msgRefuseOk()
{
	return true;
}

bool ModuleMatch::msgCopyMatchOperationCancel()
{
	return true;
}

bool ModuleMatch::enterPvpByVote(uint32 mtype)
{
	inner::InnerVoteInfo vote;
	vote.set_mtype(mtype);
	vote.set_enter(cTeam::eEnterByMatch);
	auto& copy = *vote.mutable_copy();
	auto& pvp = *copy.mutable_pvp();
	auto funcF33 = ([&]()->bool
		{
			auto* pMatchCfg = gMatchCfg->getMatchConfig(cMatch::eMatchF33);
			if (!pMatchCfg)
			{
				Log_Error("!pMatchCfg");
				return false;
			}
			auto* pCopyCfg = gCopyCfg->getCopyConfig(pMatchCfg->copyId);
			if (!pCopyCfg)
			{
				Log_Error("!pCopyCfg");
				return false;
			}
			auto* pCopyLevelCfg = gCopyCfg->getFirstLevelConfig(pCopyCfg->tid);
			if (!pCopyLevelCfg)
			{
				Log_Error("!pCopyLevelCfg");
				return false;
			}
			//这里通过系统变量,通过配置配的时间,来确定当前是local/group
			//copy.set_procfive(var.timeRef().getProcFive());
			copy.set_copyid(pMatchCfg->copyId);
			copy.set_copylvid(pCopyLevelCfg->tid);
			auto& f33 = *pvp.mutable_f33();
			f33.set_copyid(pMatchCfg->copyId);
			return true;
		});
	auto funcF55 = ([&]()->bool
		{
			auto* pMatchCfg = gMatchCfg->getMatchConfig(cMatch::eMatchF55);
			if (!pMatchCfg)
			{
				Log_Error("!pMatchCfg");
				return false;
			}
			auto* pCopyCfg = gCopyCfg->getCopyConfig(pMatchCfg->copyId);
			if (!pCopyCfg)
			{
				Log_Error("!pCopyCfg");
				return false;
			}
			auto* pCopyLevelCfg = gCopyCfg->getFirstLevelConfig(pCopyCfg->tid);
			if (!pCopyLevelCfg)
			{
				Log_Error("!pCopyLevelCfg");
				return false;
			}
			//这里通过系统变量,通过配置配的时间,来确定当前是local/group
			//copy.set_procfive(var.timeRef().getProcFive());
			copy.set_copyid(pMatchCfg->copyId);
			copy.set_copylvid(pCopyLevelCfg->tid);
			auto& f33 = *pvp.mutable_f55();
			f33.set_copyid(pMatchCfg->copyId);
			return true;
		});
	bool isOk = false;
	switch (mtype)
	{
	case cMatch::eMatchF33: isOk = funcF33(); break;
	case cMatch::eMatchF55: isOk = funcF55(); break;
	}
	if (!isOk)
	{
		return false;
	}
	//发送到teamsvr进行投票(可能跨服组队,即使本服组队,scene分布式,同服玩家散落在不同scenesvr进程上)
	//return _pUser->msgsendCmptRef().sendCmdToLocalTeamUser([&](
	//	auto& ntf = *out.mutable_fromscene_enterbyvote();
	//	*ntf.mutable_vote() = std::move(vote);
	//	));
}