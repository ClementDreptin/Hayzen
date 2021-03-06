#include "pch.h"
#include "Games\MW2\Menu.h"

#include "Games\MW2\Functions.h"

namespace MW2
{
	Menu::Menu(int clientNum)
		: m_ClientNum(clientNum), m_Open(false), m_CurrentScrollerPos(0)
	{
		m_Background = HudElem_Alloc(clientNum, 0);
		SetShader(m_Background, "white", m_MenuX, m_MenuY, m_MenuWidth, m_MenuHeight, COLOR_BLACK_NO_ALPHA);

		m_Title = HudElem_Alloc(clientNum, 0);
		SetText(m_Title, "Cod Jumper", 3.0f, m_MenuX + m_MenuWidth / 2, m_MenuY + m_Padding, COLOR_WHITE_NO_ALPHA);

		m_Scroller = HudElem_Alloc(clientNum, 0);
		SetShader(m_Scroller, "white", m_MenuX, m_MenuY + (m_Padding * 2) + m_TitleHeight, m_MenuWidth, m_LineHeight, COLOR_WHITE_NO_ALPHA, 0.5f);

		GoToRoot();
	}

	void Menu::GoToRoot()
	{
		Cleanup();

		// m_Open will indicate if it's the first time the method is called or not
		m_Options.push_back(Option(m_ClientNum, "Main", 0, m_Open));
		m_Options.push_back(Option(m_ClientNum, "Teleport", 1, m_Open));
		m_Options.push_back(Option(m_ClientNum, "Infect", 2, m_Open));
	}

	void Menu::GoToMain()
	{
		Cleanup();

		m_Options.push_back(Option(m_ClientNum, "Sub Option 1", 0, true));
		m_Options.push_back(Option(m_ClientNum, "Sub Option 2", 1, true));
	}

	void Menu::GoToTeleport()
	{
		Cleanup();

		m_Options.push_back(Option(m_ClientNum, "Sub Option 3", 0, true));
		m_Options.push_back(Option(m_ClientNum, "Sub Option 4", 1, true));
	}

	void Menu::GoToInfect()
	{
		Cleanup();

		m_Options.push_back(Option(m_ClientNum, "Sub Option 5", 0, true));
		m_Options.push_back(Option(m_ClientNum, "Sub Option 6", 1, true));
	}

	void Menu::Cleanup()
	{
		for (size_t i = 0; i < m_Options.size(); i++)
			m_Options[i].FreeHudElem();

		m_CurrentScrollerPos = 0;
		MoveScroller(m_CurrentScrollerPos);

		m_Options.clear();
	}

	void Menu::OptionClicked()
	{
		iPrintLn(m_ClientNum, "^2Option Clicked");
	}

	void Menu::OnAPressed(const std::string& optionName)
	{
		if (optionName == "Main")
			GoToMain();
		if (optionName == "Teleport")
			GoToTeleport();
		if (optionName == "Infect")
			GoToInfect();
		if (optionName == "Sub Option 1")
			OptionClicked();
		if (optionName == "Sub Option 2")
			OptionClicked();
		if (optionName == "Sub Option 3")
			OptionClicked();
		if (optionName == "Sub Option 4")
			OptionClicked();
		if (optionName == "Sub Option 5")
			OptionClicked();
		if (optionName == "Sub Option 6")
			OptionClicked();
	}

	void Menu::OnBPressed(const std::string& optionName)
	{
		if (optionName == "Sub Option 1")
			GoToRoot();
		if (optionName == "Sub Option 2")
			GoToRoot();
		if (optionName == "Sub Option 3")
			GoToRoot();
		if (optionName == "Sub Option 4")
			GoToRoot();
		if (optionName == "Sub Option 5")
			GoToRoot();
		if (optionName == "Sub Option 6")
			GoToRoot();
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

		if (eventString == "dpad_up" && m_Open)
		{
			m_CurrentScrollerPos--;

			if (m_CurrentScrollerPos < 0)
				m_CurrentScrollerPos = m_Options.size() - 1;

			MoveScroller(m_CurrentScrollerPos);
		}

		if (eventString == "dpad_down" && m_Open)
		{
			m_CurrentScrollerPos++;

			if (m_CurrentScrollerPos >= (int)m_Options.size())
				m_CurrentScrollerPos = 0;

			MoveScroller(m_CurrentScrollerPos);
		}

		if (eventString == "A" && m_Open)
			OnAPressed(m_Options[m_CurrentScrollerPos].GetName());

		if (eventString == "B" && m_Open)
			OnBPressed(m_Options[m_CurrentScrollerPos].GetName());
	}

	void Menu::Open()
	{
		MakeAppear(m_Background, 180);
		MakeAppear(m_Title);
		MakeAppear(m_Scroller, 180);

		for (size_t i = 0; i < m_Options.size(); i++)
			m_Options[i].Activate();

		m_Open = true;
	}

	void Menu::Close()
	{
		MakeDisappear(m_Background);
		MakeDisappear(m_Title);
		MakeDisappear(m_Scroller);

		for (size_t i = 0; i < m_Options.size(); i++)
			m_Options[i].Deactivate();

		m_Open = false;
	}

	void Menu::MoveScroller(int position)
	{
		m_Scroller->elem.y = (float)(m_MenuY + (m_Padding * 2) + m_TitleHeight + (m_LineHeight * position));
	}
}