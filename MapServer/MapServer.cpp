#include  "MapLogic.h"
#include "log_mgr.h"
#include "func_proc.h"

int main(int argc, char* argv[])
{
	Log_Custom("start", "start map server!!!");
	if (gMapLogicIns)
	{
		if (!gMapLogicIns->init())
		{
			gMapLogicIns->stop();
			gMapLogicIns->fini();
			return -1;
		}
		std::string strLocalIp = get_local_ip();
		Log_Custom("start", "local ip:%s", strLocalIp.c_str());
		gMapLogicIns->run();
		gMapLogicIns->fini();
	}
	return 0;
}