#include "pch.h"
#include "Games\MW2\Menu.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	Menu::Menu(int clientNum, int teamNum)
		: m_Open(false)
	{
		m_Background = HudElem_Alloc(clientNum, teamNum);
		SetShader(m_Background, "black", (float)m_X, (float)m_Y, m_Width, m_Height, 0, 0, 0, 0);

		m_Title = HudElem_Alloc(clientNum, teamNum);
		SetText(m_Title, "Main Menu", 3, (float)m_X, 10.0f, 255, 255, 255, 0);

		m_Options.push_back(Option(clientNum, teamNum, "Option 1", (float)m_X, 10.0f + m_LineHeight * 1));
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