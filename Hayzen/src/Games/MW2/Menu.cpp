#include "pch.h"
#include "Games\MW2\Menu.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	Menu::Menu(int clientNum, int teamNum)
		: m_Open(false)
	{
		m_Background = HudElem_Alloc(clientNum, teamNum);
		m_Title = HudElem_Alloc(clientNum, teamNum);
	}

	void Menu::OnEvent(const std::string& eventString)
	{
		if (eventString == "dpad_left")
		{
			if (!m_Open)
				Open();
			else
				Close();
		}
	}

	void Menu::Open()
	{
		SetShader(m_Background, "black", 591.0f, 240.0f, 300, 470, 5, 0, 0.0f, 0, 0, 0, 180);
		SetText(m_Title, "Main Menu", 4, 3, 591.0f, 10.0f, 5, 1);
		m_Open = true;
	}

	void Menu::Close()
	{
		MakeDisappear(m_Background);
		MakeDisappear(m_Title);
		m_Open = false;
	}
}