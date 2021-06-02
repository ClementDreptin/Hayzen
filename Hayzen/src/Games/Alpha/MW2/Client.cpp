#include "pch.h"
#include "Games\Alpha\MW2\Client.h"

namespace Alpha
{
namespace MW2
{
	Client::Client(int clientNum)
		: m_Num(clientNum)
	{
		m_Menu = Menu(clientNum);
		m_Initialized = true;
	}
}
}