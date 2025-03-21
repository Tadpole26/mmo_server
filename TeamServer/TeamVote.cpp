#include "common.pb.h"
#include "team.pb.h"
#include "innercommon.pb.h"

#include "TeamVote.h"
#include "TeamUser.h"
#include "TeamMember.h"
#include "TeamLeader.h"
#include "TeamUserMgr.h"
#include "SessionTeam.h"
#include "TeamMgr.h"
#include "CopyConfig.h"
#include "TeamConfig.h"

VoteUserData::~VoteUserData() 
{
}

void VoteUserData::fill(common::RoleVoteState& out) const
{
	out.set_roleid(roleId);
	out.set_state(method);
}

TeamVote::TeamVote(zVoteIdType uid)
{
}

TeamVote::~TeamVote()
{
}

void TeamVote::final()
{
}

void TeamVote::fill(common::VoteBaseData& out) const
{
	out.set_expiretime(_expireTime);
	out.set_lastingtime(_lastingTime);
}

void TeamVote::fill(common::RoleVoteStateList& out) const
{
	for (auto& vote : _votes)
	{
		vote.fill(*out.add_roles());
	}
}

void TeamVote::fill(inner::InnerCopyCrit& out) const
{
	out.set_hashidf(11);
	out.set_critid(_uid);
	out.set_expire(_expireTime);
}

void TeamVote::fill(const inner::InnerCopyEnter& enter, inner::InnerCopyType& out) const
{
	out.set_copylvid(enter.copylvid());
	out.set_eproc(enter.eproc());
	out.set_threeid(enter.threeid());
	*out.mutable_pve() = *_pve;
}

void TeamVote::fill(const inner::InnerCopyEnter& enter, inner::InnerCopyWay& out) const
{
	*out.mutable_teamlist() = enter.teamlist();
	auto& byteam = *out.mutable_byteam();
	if (enter.has_chglvid())
	{
	}
}

bool TeamVote::tick()
{
	if (isOver())
	{
		if (getExpireTime() > GetCurrTime()) return false;
		foreachVoter([&](auto& voter) -> bool
			{
				auto* pUser = gTeamUserMgr->getRole(voter.roleId);
				if (!pUser) return true;
				pUser->setVotePtr(nullptr);
				return true;
			});
		_votes.clear();
		return true;
	}
	if (getExpireTime() <= GetCurrTime())
		expireStopVote();
	return false;
}

void TeamVote::setCopyPve(const inner::InnerCopyPve& in)
{
	*_pve = in;
}

void TeamVote::setSceneId(zSceneIdType sceneId)
{
	_sceneId = sceneId;
}

void TeamVote::startLoading()
{

}

bool TeamVote::addVoter(TeamUser& user)
{
	auto iter = std::find_if(_votes.begin(), _votes.end(), [&](const VoteUserData& data) { return user.getRoleId() == data.roleId; });
	if (iter == _votes.end())
	{
		_votes.emplace_back(user.getRoleId());
	}
	return true;
}

bool TeamVote::addVoter(TeamUser& user, bool right)
{
	auto iter = std::find_if(_votes.begin(), _votes.end(), [&](const VoteUserData& data) { return user.getRoleId() == data.roleId; });
	if (iter == _votes.end())
	{
		_votes.emplace_back(user.getRoleId());
		auto& voteData = _votes.back();
		voteData.right = right;
	}
	return true;
}

void TeamVote::startVote()
{
	if (checkHandle)
	{
		_state = VoteState_Check;
		_startTime = GetCurrTime();
		_expireTime = _startTime + 10;
	}
	else
	{
		_state = VoteState_Vote;
		_startTime = GetCurrTime();
		_expireTime = _startTime + _lastingTime;
		if (beginHandle) beginHandle();
	}
}

void TeamVote::stopVote()
{
	if (isOver()) return;
	uint32 ret = VoteResult_None;
	for (auto& result : _votes)
	{
		if (!result.right)
			continue;
		if (result.method == VoteMethod_None)
		{
			if (!_agreeByDefault)
				ret = VoteResult_Reject;
			else
				ret = VoteResult_Pass;
		}
		else if (result.method == VoteMethod_Oppose)
		{
			ret = VoteResult_Reject;
		}
		else if (result.method == VoteMethod_Agree)
		{
			ret = VoteResult_Pass;
		}
	}
	if (ret == VoteResult_None)
		ret = VoteResult_Reject;
	__voteOver(ret);
}

void TeamVote::tryPassVote()
{
	bool pass = true;
	for (auto& result : _votes)
	{
		if (result.method != VoteMethod_Agree)
		{
			pass = false;
			return;
		}
	}
	__voteOver(VoteResult_Pass);
}

void TeamVote::expireStopVote()
{
	stopVote();
}

void TeamVote::confirmVote()
{
	_state = VoteState_Confirm;
	_confirmTime = GetCurrTime();
}

void TeamVote::userCheck(zRoleIdType roleId, bool success)
{
	if (isOver()) return;
	auto iter = std::find_if(_votes.begin(), _votes.end(), [&](const VoteUserData& data) { return roleId == data.roleId; });
	if (iter == _votes.end()) return;
	auto& userdata = *iter;
	userdata.checked = success ? 1 : 2;
	bool pass = true;
	for (auto& userdata : _votes)
	{
		if (!userdata.checked) return;
		if (userdata.checked == 2) pass = false;
	}
	if (pass)
	{
		_state = VoteState_Vote;
		_startTime = GetCurrTime();
		_expireTime = _startTime + _lastingTime;

		if (beginHandle) beginHandle();
	}
	else
	{
		__voteOver(VoteResult_CheckFail);
		if (checkFailHandle) checkFailHandle();
	}
}

void TeamVote::userAgree(TeamUser& user)
{
	__userVote(user, VoteMethod_Agree);
}

void TeamVote::userOppose(TeamUser& user)
{
	__userVote(user, VoteMethod_Oppose);
}

void TeamVote::broadCast(const uint32 first, const uint32 second, const google::protobuf::Message& in)
{
	for (auto& vote : _votes)
	{
		auto* pUser = gTeamUserMgr->getRole(vote.roleId);
		if (!pUser) continue;
		//pUser->sendCmdToMe()
	}
}

bool TeamVote::foreachVoter(std::function<bool(const VoteUserData&)>&& func)
{
	return std::all_of(_votes.begin(), _votes.end(), [&](const auto& vote) { return func(vote); });
}

void TeamVote::__userVote(TeamUser& user, uint32 method)
{
	if (isOver()) return;
	auto iter = std::find_if(_votes.begin(), _votes.end(), [&](const VoteUserData& data) { return user.getRoleId() == data.roleId; });
	if (iter == _votes.end()) return;
	iter->method = method;
	if (!updateHandle) return;
	updateHandle(*iter);

	if (!iter->right) return;
	if (method == VoteMethod_Oppose)			//一票否决
	{
		stopVote();
	}
	else
	{
		tryPassVote();
	}
}

void TeamVote::__voteOver(uint32 result)
{
	_state = VoteState_Over;
	_expireTime = GetCurrTime();
	_result = result;
	switch (_result)
	{
		case VoteResult_Pass:
		{
			if (passHandle) passHandle();
			_expireTime = GetCurrTime() + 5;
		}
		break;
		case VoteResult_Reject:
		{
			if (rejectHandle) rejectHandle();
		}
		break;
		case VoteResult_CheckFail:
		{
			Log_Error("vote result check fail");
		}
		break;
		default:
		{
			Log_Error("no result");
		}
		break;
	}
}

bool TeamUser::innerPersonVote(bool agree)
{
	return true;
}

bool TeamUser::innerEnterByVote(const inner::InnerCopyPve& pve, const inner::InnerCopyEnter& enter)
{
	const auto* cfgLvId = gCopyCfg->getCopyLevelConfig(enter.copylvid());
	if (cfgLvId == nullptr)
	{
		return false;
	}
	std::vector<TeamUser*> voterList;
	auto funcNotTeam = ([&]()->bool
		{
			if (pUnit != nullptr)
			{
				return false;
			}
			if (isTeamPersonMatching())
			{
				return false;
			}
			if (isVoting())
			{
				return false;
			}
			return true;
		});
	auto funcHasTeam = ([&](SessionTeam& ref)->bool
		{
			if (ref.isMatching())
			{
				//ref.foreachMem([](auto* memRef) {systips.send(*memRef.pUser, Tips_Match_TeamIsTeamMatching) / return true; });
				return false;
			}
			if (ref.isVoting())
			{
				//ref.foreachMem([](auto* memRef) {systips.send(*memRef.pUser, Tips_Match_TeamIsTeamMatching) / return true; });
				return false;
			}
			if (!ref.foreachMem([](auto& memRef)->bool
				{
					return !(memRef.pUser->pUnit != nullptr);
				}))
			{
				//ref.foreachMem([](auto* memRef) {systips.send(*memRef.pUser, Tips_Match_TeamIsTeamMatching) / return true; });
				return false;
			}
			return true;
		});

	//组队副本
	auto funcForTeam = ([&](const inner::InnerCopyTeamList& teamList)->bool
		{
			if (pMem == nullptr)
			{
				return false;
			}
			if (!pMem->isLeader())
			{
				return false;
			}
			if (!funcHasTeam(pMem->teamRef()))
			{
				return false;
			}
			if (teamList.teams_size() != 1)
			{
				return false;
			}
			const auto& one = teamList.teams(0);
			if (one.roles_size() != pMem->teamRef().sizeMember())
			{
				return false;
			}
			for (auto i = 0; i < one.roles_size(); ++i)
			{
				auto* pTeamMem = pMem->teamRef().getMember(one.roles(i).roleid());
				if (pTeamMem == nullptr)
				{
					return false;
				}
				voterList.push_back(pTeamMem->pUser);
			}
			return true;
		});
	//家园副本(单人进入)
	auto funcForHome = ([&](const inner::InnerCopyTeamList& teamList)->bool
		{
			if (pMem == nullptr)
			{
				if (!funcNotTeam())
				{
					return false;
				}
			}
			else
			{
				if (!funcHasTeam(pMem->teamRef()))
				{
					return false;
				}
			}
			if (teamList.teams_size() != 1)
			{
				return false;
			}
			const auto& one = teamList.teams(0);
			if (one.roles_size() != 1)
			{
				return false;
			}
			if (one.roles(0).roleid() != getRoleId())
			{
				return false;
			}
			voterList.push_back(this);
			return true;
		});
	bool isOk = false;
	switch (enter.type_case())
	{
	case inner::InnerCopyEnter::TypeCase::kTeam: isOk = funcForTeam(enter.teamlist()); break;
	case inner::InnerCopyEnter::TypeCase::kHome: isOk = funcForHome(enter.teamlist()); break;
	}
	if (!isOk)
	{
		return false;
	}
	if (voterList.empty())
	{
		return false;
	}
	//创建投票
	TeamVote* pVote = nullptr;
	const config::vote_info_t* cfg = nullptr;
	if (enter.voteid() && voterList.size() > 1 && enter.type_case() != inner::InnerCopyEnter::TypeCase::kChgLvId)
	{
		if ((cfg = gTeamCfg->getVoteConfig(enter.voteid())) != nullptr)
		{
			pVote = gTeamMgr->createVote(cfg->lastTime, cfg->defaultVote);
		}
	}
	else
	{
		//单人无需投票,直接通过
		pVote = gTeamMgr->createVote();
	}
	if (pVote == nullptr)
	{
		return false;
	}
	pVote->setCopyPve(pve);
	for (auto* pVoter : voterList)
	{
		pVote->addVoter(*pVoter);
		pVoter->setVotePtr(pVote);
	}

	auto funcHandleForTeam = ([&]() 
	{
		pVote->passHandle = [=]()
			{
				if (cfg)
				{
					client::ModuleTeam_Ntf_VoteEnd ntf;
					ntf.set_pass(true);
					//pVote->broadcase();
				}
				//跳转进入副本
			};
		if (cfg)
		{
			pVote->beginHandle = [=]()
				{
						
					client::ModuleTeam_Ntf_VoteBegin ntf;
					//pVote->fill(*ntf.mutable_base());
					//pVote->broadcase();
						
				};
			pVote->userAgree(*this);
			pVote->updateHandle = [=](const VoteUserData& role)
				{
					client::ModuleTeam_Ntf_VoteUpdate ntf;
					//pVote->fill(*ntf.mutable_base());
					//pVote->broadcase();
				};
			pVote->rejectHandle = [=]()
				{
					client::ModuleTeam_Ntf_VoteEnd ntf;
					//pVote->fill(*ntf.mutable_base());
					//pVote->broadcase();
				};
		}
	});
	auto funcHandleForHome = ([&]() 
	{
		pVote->passHandle = [=]()
		{
			client::ModuleTeam_Ntf_VoteEnd ntf;
			ntf.set_pass(true);
			//pVote->broadcase();
			//跳转进入副本
		};
	});
	switch (enter.type_case())
	{
	case inner::InnerCopyEnter::TypeCase::kTeam: funcHandleForTeam(); break;
	case inner::InnerCopyEnter::TypeCase::kHome: funcHandleForHome(); break;
	}
	for (auto* pUser : voterList)
	{
		pVote->userCheck(pUser->getRoleId(), true);
	}
	pVote->startVote();
	return true;
}