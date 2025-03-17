#include  "TeamLogic.h"
#include "cLogMgr.h"
#include "func_proc.h"

int main(int argc, char* argv[])
{
	Log_Custom("start", "start map server!!!");
	if (gTeamLogic)
	{
		if (!gTeamLogic->init())
		{
			gTeamLogic->stop();
			gTeamLogic->fini();
			return -1;
		}
		std::string strLocalIp = get_local_ip();
		Log_Custom("start", "local ip:%s", strLocalIp.c_str());
		gTeamLogic->run();
		gTeamLogic->fini();
	}
	return 0;
}