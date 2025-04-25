
#include <iostream>
#include "MatchLogic.h"
#include "func_proc.h"
#include "QueueMgrF55.h"

int main()
{
	Log_Custom("start", "start match server!!!");
	if (gMatchLogic)
	{
		if (!gMatchLogic->init())
		{
			gMatchLogic->stop();
			gMatchLogic->fini();
			return -1;
		}
		QueueMgrF55 f55(1);
		f55.init();
		std::string strLocalIp = get_local_ip();
		Log_Custom("start", "local ip:%s", strLocalIp.c_str());
		gMatchLogic->run();
		gMatchLogic->fini();
	}
	return 0;
}
