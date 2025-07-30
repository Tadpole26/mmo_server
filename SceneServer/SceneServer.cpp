#include  "SceneLogic.h"
#include "func_proc.h"

int main(int argc, char* argv[])
{
	Log_Custom("start", "start scene server!!!");
	if (gSceneLogic)
	{
		if (!gSceneLogic->init())
		{
			gSceneLogic->stop();
			gSceneLogic->fini();
			return -1;
		}
		std::string strLocalIp = get_local_ip();
		Log_Custom("start", "local ip:%s", strLocalIp.c_str());
		gSceneLogic->run();
		gSceneLogic->fini();
	}
	return 0;
}
