#include "CGateLogic.h"
#include "zLogMgr.h"

int main(int argc, char* argv[])
{
	Log_Custom("start", "start GateServer!!!");
	//GateServer* pLogic = GateServer::GetInstance();
	if (gGateLogicIns)
	{
		if (!gGateLogicIns->Arg(argc, argv))
			return -1;
		if (!gGateLogicIns->Launch())
			return -1;
	}
	return 0;
}


