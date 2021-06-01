#pragma once

#include "Games\SpecOps\MW3\Menu.h"

namespace SpecOps
{
namespace MW3
{
	class Client
	{
	public:
		Client() : m_Num(-1), m_Initialized(false) {}
		Client(int clientNum);

		inline Menu& GetMenu() { return m_Menu; }
		inline bool IsInitialized() { return m_Initialized; }
	private:
		int m_Num;
		Menu m_Menu;
		bool m_Initialized;
	};
}
}