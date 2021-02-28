#include "pch.h"
#include "Games\MW2\Menu.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	Menu::Menu(int clientNum, int teamNum)
		: m_Open(false)
	{
		m_Background = HudElem_Alloc(clientNum, teamNum);
	}

	void Menu::Open()
	{
		SetShader(m_Background, "black", 591.0f, 240.0f, 300, 470, 5, 0, 0.0f, 0, 0, 0, 180);
		m_Open = true;
	}

	void Menu::Close()
	{
		MakeDisappear(m_Background);
		m_Open = false;
	}
}