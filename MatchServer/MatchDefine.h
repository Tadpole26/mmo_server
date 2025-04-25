#pragma once
#include "sdk.h"

class MatchQueue;
using MatchQueueActiveListT = std::list<MatchQueue*>;
using MatchQueueActiveListItT = MatchQueueActiveListT::iterator;
using MatchQueueActivePairT = std::pair<bool, MatchQueueActiveListItT>;
using MatchQueueMapT = std::unordered_map<uint64, MatchQueue*>;
//队伍数量
using MatchNumSideT = std::vector<uint32>;
using MatchNumSideVecT = std::vector<MatchNumSideT>;
using MatchNumSideAutoId2StrMapT = std::unordered_map<uint32, std::string>;
using MatchNumSideAutoId2VecMapT = std::unordered_map<uint32, MatchNumSideT>;
using MatchNumSideStr2AutoIdMapT = std::unordered_map<std::string, uint32>;
using MatchNumSideStr2VecMapT = std::unordered_map<std::string, MatchNumSideT>;
using MatchNumFormStrPVPT = std::pair<std::string, std::string>;
using MatchNumFormStrPVET = std::string;

//队伍数量职业
using TTTimeVecT = std::vector<std::pair<uint32, uint32>>;
using TTNumCareerSideT = std::vector<uint32>;
using TTNumCareerSidePairT = std::pair<std::vector<uint32>, std::vector<uint32>>;
using TTNumCareerFormT = std::pair<TTNumCareerSideT, TTNumCareerSideT>;
using TTNumCareerFormStrT = std::pair<std::string, std::string>;
using TTNumCareerFormAutoIdT = std::pair<uint32, uint32>;
using TTNumCareerSideAutoId2StrMapT = std::unordered_map<uint32, std::string>;
using TTNumCareerSideAutoId2VecMapT = std::unordered_map<uint32, TTNumCareerSideT>;
using TTNumCareerSideStr2AutoIdMapT = std::unordered_map<std::string, uint32>;
using TTNumCareerSideStr2VecMapT = std::unordered_map<std::string, TTNumCareerSideT>;
