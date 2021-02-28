#include "pch.h"
#include "Games\MW2\Menu.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	Menu::Menu(int clientNum, int teamNum)
		: m_Open(false), m_CurrentScrollerPos(0)
	{
		m_Background = HudElem_Alloc(clientNum, teamNum);
		SetShader(m_Background, "black", m_MenuX, m_MenuY, m_MenuWidth, m_MenuHeight, 0, 0, 0, 0);

		m_Title = HudElem_Alloc(clientNum, teamNum);
		SetText(m_Title, "Cod Jumper", 3.0f, m_MenuX + m_MenuWidth / 2, m_MenuY + m_Padding, 255, 255, 255, 0);

		m_Scroller = HudElem_Alloc(clientNum, teamNum);
		SetShader(m_Scroller, "white", m_MenuX, m_MenuY + (m_Padding * 2) + m_TitleHeight, m_MenuWidth, m_LineHeight, 255, 255, 255, 0);

		m_Options.push_back(Option(clientNum, teamNum, "Main", 0));
		m_Options.push_back(Option(clientNum, teamNum, "Teleport", 1));
		m_Options.push_back(Option(clientNum, teamNum, "Infect", 2));
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

		if (eventString == "dpad_up")
		{
			m_CurrentScrollerPos--;

			if (m_CurrentScrollerPos < 0)
				m_CurrentScrollerPos = m_Options.size() - 1;

			MoveScroller(m_CurrentScrollerPos);
		}

		if (eventString == "dpad_down")
		{
			m_CurrentScrollerPos++;

			if (m_CurrentScrollerPos >= (int)m_Options.size())
				m_CurrentScrollerPos = 0;

			MoveScroller(m_CurrentScrollerPos);
		}
	}

	void Menu::Open()
	{
		MakeAppear(m_Background, 180);
		MakeAppear(m_Title);
		MakeAppear(m_Scroller, 180);

		for (size_t i = 0; i < m_Options.size(); i++)
			MakeAppear(m_Options[i].GetHudElem());

		m_Open = true;
	}

	void Menu::Close()
	{
		MakeDisappear(m_Background);
		MakeDisappear(m_Title);
		MakeDisappear(m_Scroller);

		for (size_t i = 0; i < m_Options.size(); i++)
			MakeDisappear(m_Options[i].GetHudElem());

		m_Open = false;
	}

	void Menu::MoveScroller(int position)
	{
		m_Scroller->elem.y = (float)(m_MenuY + (m_Padding * 2) + m_TitleHeight + (m_LineHeight * position));
	}
}