#include "ParamConfig.h"
#include "util_random.h"

namespace config
{
	bool ParamConfig::canMatchRobot(uint32 score)
	{
		return score < matchRobotMaxScore;
	}

	uint32 ParamConfig::getMinIdRobot()
	{
		return 1;
	}

	uint32 ParamConfig::getMaxIdRobot()
	{
		return getMatchRobotWaitTimeMax() - getMatchRobotWaitTimeMin();
	}

	uint32 ParamConfig::getMatchRobotWaitTimeMin()
	{
		return matchRobotWaitTime[matchRobotWaitTime.size() - 1].first;
	}

	uint32 ParamConfig::getMatchRobotWaitTimeMax()
	{
		return matchRobotWaitTime[0].second;
	}

	uint32 ParamConfig::getMatchRobotWaitTimeByCLose(uint32 cLose)
	{
		auto index = cLose;
		if (index >= matchRobotWaitTime.size())
		{
			index = matchRobotWaitTime.size();
		}
		const auto& p = matchRobotWaitTime[index];
		//auto diff = rand_range(p.first, p.second) - getMatchRobotWaitTimeMax();
		//if (diff < getMinIdRobot()) return getMinIdRobot();
		//else if (diff > getMaxIdRobot()) return getMaxIdRobot();
		//else return diff;
		return 0;
	}
}
