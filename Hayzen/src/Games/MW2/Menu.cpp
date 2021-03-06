#include "pch.h"
#include "Games\MW2\Menu.h"

#include "Games\MW2\Functions.h"
#include "Utils\Formatter.h"

namespace MW2
{
	std::unordered_map<std::string, std::vector<std::string>> Menu::s_Structure;

	Menu::Menu(int clientNum)
		: m_ClientNum(clientNum), m_Open(false), m_CurrentScrollerPos(0)
	{
		m_Background = HudElem_Alloc(clientNum, 0);
		SetShader(m_Background, "white", m_MenuX, m_MenuY, m_MenuWidth, m_MenuHeight, COLOR_BLACK_NO_ALPHA);

		m_Title = HudElem_Alloc(clientNum, 0);
		SetText(m_Title, "Cod Jumper", 3.0f, m_MenuX + m_MenuWidth / 2, m_MenuY + m_Padding, COLOR_WHITE_NO_ALPHA);

		m_Scroller = HudElem_Alloc(clientNum, 0);
		SetShader(m_Scroller, "white", m_MenuX, m_MenuY + (m_Padding * 2) + m_TitleHeight, m_MenuWidth, m_LineHeight, COLOR_WHITE_NO_ALPHA, 0.5f);

		CreateStructure();

		GoToMenu("root");
	}

	void Menu::OptionClicked(const std::string& optionName)
	{
		iPrintLn(m_ClientNum, Formatter::Format("^2You clicked on %s", optionName.c_str()));
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

	void Menu::CreateStructure()
	{
		s_Structure["root"] = std::vector<std::string>();
		s_Structure["root"].reserve(3);
		s_Structure["root"].emplace_back("Main");
		s_Structure["root"].emplace_back("Teleport");
		s_Structure["root"].emplace_back("Infect");

		s_Structure["Main"] = std::vector<std::string>();
		s_Structure["Main"].reserve(2);
		s_Structure["Main"].emplace_back("Sub Option 1");
		s_Structure["Main"].emplace_back("Sub Option 2");

		s_Structure["Teleport"] = std::vector<std::string>();
		s_Structure["Teleport"].reserve(2);
		s_Structure["Teleport"].emplace_back("Sub Option 3");
		s_Structure["Teleport"].emplace_back("Sub Option 4");

		s_Structure["Infect"] = std::vector<std::string>();
		s_Structure["Infect"].reserve(2);
		s_Structure["Infect"].emplace_back("Sub Option 5");
		s_Structure["Infect"].emplace_back("Sub Option 6");
	}

	void Menu::GoToMenu(const std::string& menuName)
	{
		Cleanup();

		m_Options.reserve(s_Structure[menuName].size());

		for (size_t i = 0; i < s_Structure[menuName].size(); i++)
			m_Options.emplace_back(Option(m_ClientNum, s_Structure[menuName][i], i, m_Open));
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

	void Menu::OnAPressed(const std::string& optionName)
	{
		if (optionName == "Main" || optionName == "Teleport" || optionName == "Infect")
			GoToMenu(optionName);
		if (optionName == "Sub Option 1")
			OptionClicked(optionName);
		if (optionName == "Sub Option 2")
			OptionClicked(optionName);
		if (optionName == "Sub Option 3")
			OptionClicked(optionName);
		if (optionName == "Sub Option 4")
			OptionClicked(optionName);
		if (optionName == "Sub Option 5")
			OptionClicked(optionName);
		if (optionName == "Sub Option 6")
			OptionClicked(optionName);
	}

	void Menu::OnBPressed(const std::string& optionName)
	{
		if (optionName == "Sub Option 1")
			GoToMenu("root");
		if (optionName == "Sub Option 2")
			GoToMenu("root");
		if (optionName == "Sub Option 3")
			GoToMenu("root");
		if (optionName == "Sub Option 4")
			GoToMenu("root");
		if (optionName == "Sub Option 5")
			GoToMenu("root");
		if (optionName == "Sub Option 6")
			GoToMenu("root");
	}

	void Menu::MoveScroller(int position)
	{
		m_Scroller->elem.y = (float)(m_MenuY + (m_Padding * 2) + m_TitleHeight + (m_LineHeight * position));
	}

	void Menu::Cleanup()
	{
		for (size_t i = 0; i < m_Options.size(); i++)
			m_Options[i].FreeHudElem();

		m_CurrentScrollerPos = 0;
		MoveScroller(m_CurrentScrollerPos);

		m_Options.clear();
	}
}