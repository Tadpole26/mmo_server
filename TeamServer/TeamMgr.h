#pragma once
#include "singleton.h"
#include "TeamDefine.h"
#include "cTeam.h"

namespace common
{
	class PlatTargetTeamList;
}

namespace inner
{
	class InnerRoleIdList;
}

class TeamSizeQueueTarget
{
public:
	TeamSizeQueueTarget(uint32 key, TeamTargetQueue* pTgQueue);
	~TeamSizeQueueTarget();
	void final();
public:
	inline uint32 getKey() { return _key; }
	inline uint32 sizeTeam() { return _allTeam.size(); }
	inline bool teamEmpty() { return _allTeam.empty(); }
public:
	void addTeam(SessionTeam* team);
	void removeTeam(SessionTeam* team);
	void foreachTeam(std::function<bool(SessionTeam* team)> func);
public:
	TeamTargetQueue* pTgQueue = nullptr;
private:
	uint32 _key = 0;								// 队伍成员数量
	TeamSizeListT _allTeam;							//该成员数量队伍
};

class TeamSizeQueueLevel
{
public:
	TeamSizeQueueLevel(uint32 key, TeamLevelQueue* pLvQueue);
	~TeamSizeQueueLevel();
	void final();
public:
	inline uint32 getKey() { return _key; }
	inline uint32 sizeTeam() { return _allTeam.size(); }
	inline bool teamEmpty() { return _allTeam.size(); }
public:
	void addTeam(SessionTeam* team);
	void removeTeam(SessionTeam* team);
	SessionTeam* front();
public:
	TeamLevelQueue* pLvQueue = nullptr;
private:
	uint32 _key = 0;								//队伍成员数量
	TeamSizeListT _allTeam;							//该成员数量队伍
};

class TeamLevelQueue
{
public:
	using AllSizeQueueVecT = std::vector<TeamSizeQueueLevel*>;
	using ActivePaireT = std::pair<bool, TeamLevelQueueListItT>;
public:
	TeamLevelQueue(uint32 lv, TeamTargetQueue* pTgQueue);
	~TeamLevelQueue();
	void final();
public:
	bool tick();
	bool isActive();
	inline uint32 getKey() { return _key; }
	inline uint32 sizeUser() { return _allUser.size(); }
	TeamSizeQueueLevel* szQueuePtr(uint32 sz);
	TeamSizeQueueLevel& szQueueRef(uint32 sz);
	void addMatchUser(TeamUser* pUser);
	void removeMatchUser(TeamUser* pUser);
public:
	TeamTargetQueue* pTgQueue = nullptr;
	ActivePaireT activeLvQueuePair;					//激活位置
private:
	uint32 _key = 0;								//等级
	AllSizeQueueVecT _allSzQueue;					//正在匹配队伍(队伍人数队列)
	TeamSingleListT _allUser;						//正在匹配的个人
};

//目标队伍
class TeamTargetQueue
{
public:
	using AllSizeQueueVecT = std::vector<TeamSizeQueueTarget*>;
	using AllLevelQueueVecT = std::vector<TeamLevelQueue*>;
public:
	TeamTargetQueue(const config::team_info_t& cfg);
	~TeamTargetQueue();
	void final();
public:
	bool validTargetLevel(uint32 lv);
	uint32 levelIndex(uint32 lv);
	uint32 getTargetId();
	TeamSizeQueueTarget* szQueuePtr(uint32 sz);
	TeamSizeQueueTarget& szQueueRef(uint32 sz);
	TeamLevelQueue* lvQueuePtr(uint32 lv);
	TeamLevelQueue& lvQueueRef(uint32 lv);
	void fill(common::PlatTargetTeamList& out);
private:
	uint32 _targetId = 0;
	AllSizeQueueVecT _allSzQueue;					//1~5人对应队伍
	AllLevelQueueVecT _allLvQueue;					//minLevel~maxLevel对应队伍
};

class TeamMgr
{
public:
	static bool validMatchSize(uint32 sz);
	static uint32 sizeIndex(uint32 sz);
public:
	TeamMgr();
	~TeamMgr();
	void final();
	bool init();
	void tick();
	void fill();
public:
	using TeamIdPairT = std::pair<uint32, uint32>;
	using AllTeamMapT = std::unordered_map<zTeamIdType, SessionTeam*>;
	using AllTargetMapT = std::unordered_map<uint32, TeamTargetQueue*>;
	using TeamVoteMapT = std::unordered_map<zVoteIdType, TeamVote*> ;
	using TeamVoteListT = std::list<TeamVote*>;
public:
	SessionTeam* createTeam();
	SessionTeam* getTeam(zTeamIdType teamId);
	void destroyTeam(zTeamIdType teamId);
	//void batCreateTeam(uint32 sceneHashId, const inner::InnerCopyTeamList* in);
	void batDestroyTeam(std::initializer_list<zTeamIdType> teamList);
	void batLeaveTeam(const inner::InnerRoleIdList& roleList);
	void batLeaveTeam(std::initializer_list<TeamUser*> roleList, uint32 eLeave);
	//void batLeaveCopy(uint32 sceneHashId);
public:
	bool createTargetQueue(const config::team_info_t& cfg);
	inline uint32 sizeTgQueue() { return _allTgQueue.size(); }
	inline uint32 sizeTeam() { return _allTeam.size(); }
	TeamTargetQueue& tgQueueRef(const config::team_info_t& cfg);
	TeamTargetQueue& tgQueueRef(uint32 targetId);
	void addLvQueueToActive(TeamLevelQueue* pLvQueue);
	void removeLvQueueFromActive(TeamLevelQueue* pLvQueue);
	TeamVote* createVote();
	TeamVote* createVote(uint32 lastingTime, bool agreeByDefault);
	TeamVote* getVote(zVoteIdType voteUid);
	bool addTeamVote(TeamVote* pVote);
	void removeTeamVote(TeamVote* pVote);
	TeamVoteListItT getVoteItEnd() { return _allVoteQueue.end(); }
private:
	zTeamIdType __createTeamId();
	zVoteIdType __createVoteId();
private:
	TeamIdPairT _idpair;							//队伍id生成器
	AllTeamMapT _allTeam;							//所有创建的队伍
	AllTargetMapT _allTgQueue;						//所有目标对应队伍(用于界面展示)
	TeamLevelQueueListT _activeLvQueue;				//激活相同等级匹配
	TeamVoteMapT _allVote;						
	TeamVoteListT _allVoteQueue;
	zVoteIdType _voteIncreaseId = 0;
};

#define gTeamMgr Singleton<TeamMgr>::getInstance()


