#include "pch.h"
#include "Games\MW3\Client.h"

namespace MW3
{
	Client::Client(int clientNum)
		: m_Num(clientNum)
	{
		m_Menu = Menu(clientNum);
		m_Initialized = true;
	}
}