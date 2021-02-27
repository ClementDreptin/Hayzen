#pragma once

#include "Games\MW2\Menu.h"

namespace MW2
{
	class Client
	{
	public:
		Client(int clientNum);

		inline Menu GetMenu() const { return m_Menu; }
	private:
		Menu m_Menu;
		int m_Num;
	};
}