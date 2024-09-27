#include "CGateLogic.h"
#include "log_mgr.h"

int main(int argc, char* argv[])
{
	Log_Custom("start", "start GateServer!!!");
	//GateServer* pLogic = GateServer::GetInstance();
	if (GATE_LOGIC_INS)
	{
		if (!GATE_LOGIC_INS->Arg(argc, argv))
			return -1;
		if (!GATE_LOGIC_INS->Launch())
			return -1;
	}
	return 0;
}


