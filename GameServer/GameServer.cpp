#include  "CGameLogic.h"
#include "log_mgr.h"

int main(int argc, char* argv[])
{
	Log_Custom("start", "start game server!!!");
	if (GAME_LOGIC_INS)
	{
		if (!GAME_LOGIC_INS->Arg(argc, argv))
			return -1;
		if (!GAME_LOGIC_INS->Launch())
			return -1;
	}
	return 0;
}
