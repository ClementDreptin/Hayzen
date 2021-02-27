#include "pch.h"
#include "Games\MW2\Client.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	Client::Client(int clientNum)
		: m_Num(clientNum)
	{
		int teamNum = GetClientState(clientNum)->team;
		m_Menu = Menu(clientNum, teamNum);
	}
}