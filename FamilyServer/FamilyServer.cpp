#include "FamilyLogic.h"
#include "func_proc.h"


int main()
{
	Log_Custom("start", "start family server!!!");
	if (gFamilyLogic)
	{
		if (!gFamilyLogic->init())
		{
			gFamilyLogic->stop();
			gFamilyLogic->fini();
			return -1;
		}
		std::string strLocalIp = get_local_ip();
		Log_Custom("start", "local ip:%s", strLocalIp.c_str());
		gFamilyLogic->run();
		gFamilyLogic->fini();
	}
	return 0;
}
