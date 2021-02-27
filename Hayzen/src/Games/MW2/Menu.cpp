#include "pch.h"
#include "Games\MW2\Menu.h"

#include "Games\MW2\Functions.h"
#include "Games\MW2\Hud.h"

namespace MW2
{
	Menu::Menu(int clientNum, int teamNum)
	{
		m_HudElem = HudElem_Alloc(clientNum, teamNum);
	}

	void Menu::Open()
	{
		SetShader(m_HudElem, "black", 591.0f, 240.0f, 300, 470, 5, 0, 0.0f, 0, 0, 0, 180);
	}

	void Menu::Close()
	{
		SetShader(m_HudElem, "black", 0.0f, 0.0f, 0, 0, 5, 0, 0.0f, 0, 0, 0, 0);
	}
}