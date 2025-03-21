#pragma once
#include "sdk.h"

class TeamApply;
class TeamUser;
class TeamMember;
class TeamLeader;
class SessionTeam;
class TeamLevelQueue;
class TeamTargetQueue;
class TeamSizeQueueTarget;
class TeamSizeQueueLevel;
class TeamSizeQueueAllTarget;
class TeamVote;

using TeamSizeListT = std::list<SessionTeam*>;
using TeamSizeQueueItT = TeamSizeListT::iterator;
using TeamSingleListT = std::list<TeamUser*>;
using TeamSingleListItT = TeamSingleListT::iterator;
using TeamLevelQueueListT = std::list<TeamLevelQueue*>;
using TeamLevelQueueListItT = TeamLevelQueueListT::iterator;
using AutoRecruitTeamListT = std::list<SessionTeam>;
using AutoRecruitTeamListItT = AutoRecruitTeamListT::iterator;
using TeamVoteListT = std::list<TeamVote*>;
using TeamVoteListItT = TeamVoteListT::iterator;
