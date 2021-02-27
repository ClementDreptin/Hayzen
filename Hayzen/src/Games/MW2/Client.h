#pragma once

#include "Games\MW2\Menu.h"

namespace MW2
{
	class Client
	{
	public:
		Client() {}
		Client(int clientNum);

		inline Menu& GetMenu() { return m_Menu; }
	private:
		int m_Num;
		Menu m_Menu;
	};
}