#include "teamcommon.pb.h"
#include "innercommon.pb.h"
#include "team.pb.h"
#include "innerteam.pb.h"

#include "TeamConfig.h"
#include "CopyConfig.h"

#include "TeamApply.h"
#include "TeamUser.h"
#include "TeamUserMgr.h"
#include "TeamMember.h"
#include "TeamLeader.h"
#include "SessionTeam.h"
#include "TeamMgr.h"

bool TeamUser::innerEnterByVote(const inner::InnerVoteInfo& vote)
{
	inner::InnerCopyType ct(vote.copy());
	const auto* pCopyLvCfg = gCopyCfg->getCopyLevelConfig(ct.copylvid());
	if (!pCopyLvCfg)
	{
		return false;
	}
	if (pCopyLvCfg->copyId != ct.copyid())
	{
		return false;
	}

}