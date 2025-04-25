#include "MatchConfig.h"

namespace config
{
	bool MatchConfig::LoadConfig(const std::string& strContent)
	{
		InitSheet("match_info", std::bind(&MatchConfig::AddMatchInfo, this));

		return LoadCfgFromStr(strContent);

	}

	bool MatchConfig::AddMatchInfo()
	{
		uint32 tid = GetDataUInt("id");
		match_info_t& stElem = _matchInfoCfg[tid];
		return true;
	}

	match_info_t* MatchConfig::getMatchConfig(const uint32 tid)
	{
		auto iter = _matchInfoCfg.find(tid);
		if (iter == _matchInfoCfg.end())
			return nullptr;
		return &(iter->second);
	}

	match_dan_t* MatchConfig::getDanByScore(const uint32 score)
	{
		//¶ÁÈ¡ÅÐ¶Ï²»Îª¿Õ
		for (auto& pair : _matchDanCfg)
		{
			match_dan_t* pCfg = pair.second;
			if (score < pCfg->danUpExp)
				return pCfg;
		}
		return (_matchDanCfg.rbegin())->second;
	}

	uint32 MatchConfig::getMaxScore() const
	{
		return (_matchDanCfg.rbegin())->second->danUpExp;
	}
}
