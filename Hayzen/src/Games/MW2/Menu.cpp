#include "pch.h"
#include "Games\MW2\Menu.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	Menu::Menu(int clientNum, int teamNum)
		: m_Open(false)
	{
		m_Background = HudElem_Alloc(clientNum, teamNum);
		SetShader(m_Background, "black", m_MenuX, m_MenuY, m_MenuWidth, m_MenuHeight, 0, 0, 0, 0);

		m_Title = HudElem_Alloc(clientNum, teamNum);
		SetText(m_Title, "Main Menu", 3.0f, m_MenuX, m_Padding, 255, 255, 255, 0);

		m_Options.push_back(Option(clientNum, teamNum, "Option 1", 0));
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
		MakeAppear(m_Background, 180);
		MakeAppear(m_Title);

		for (size_t i = 0; i < m_Options.size(); i++)
			MakeAppear(m_Options[i].GetHudElem());

		m_Open = true;
	}

	void Menu::Close()
	{
		MakeDisappear(m_Background);
		MakeDisappear(m_Title);

		for (size_t i = 0; i < m_Options.size(); i++)
			MakeDisappear(m_Options[i].GetHudElem());

		m_Open = false;
	}
}