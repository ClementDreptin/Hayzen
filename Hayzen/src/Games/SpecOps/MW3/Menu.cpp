#include "pch.h"
#include "Games\SpecOps\MW3\Menu.h"

#include "Games\SpecOps\MW3\Functions.h"
#include "Games\SpecOps\MW3\MW3.h"
#include "Core\Events.h"

namespace SpecOps
{
namespace MW3
{
	const float Menu::s_MenuX = 875.0f;
	const float Menu::s_MenuY = 5.0f;
	const float Menu::s_MenuWidth = 400.0f;
	const float Menu::s_MenuHeight = 710.0f;
	const float Menu::s_Padding = 20.0f;
	const float Menu::s_TitleHeight = 60.0f;
	const float Menu::s_LineHeight = 40.0f;

	Menu::Menu(int clientNum)
		: m_ClientNum(clientNum), m_Open(false), m_CurrentScrollerPos(0), m_SavedPos(vec3(0.0f, 0.0f, 0.0f)), m_BindsEnabled(false)
	{
		m_Background = RectangleElem(s_MenuX, s_MenuY, s_MenuWidth, s_MenuHeight, HudElem::s_ColorBlack);
		m_Background.SetAlpha(0.7f);

		m_Title = TextElem("Cod Jumper", s_MenuX + s_Padding, s_Padding + s_TitleHeight, HudElem::s_ColorWhite, 1.7f);

		m_Scroller = RectangleElem(s_MenuX, s_MenuY + (s_Padding * 2) + s_TitleHeight, s_MenuWidth, s_LineHeight, HudElem::s_ColorWhite);
		m_Scroller.SetAlpha(0.7f);

		m_Instructions = TextElem("Navigate: UP - DOWN | Select: X | Back: RS",
			s_MenuX + s_Padding, s_MenuY + s_MenuHeight - s_Padding - 80, HudElem::s_ColorWhite, 0.7f);

		CreateStructure();
	}

	void Menu::ToggleGodMode()
	{
		playerState_s* playerState = SV_GetPlayerstateForClientNum(m_ClientNum);

		if (playerState->otherFlags == 0)
		{
			playerState->otherFlags = 1;
			iPrintLn(m_ClientNum, "God Mode ^2On");
		}
		else
		{
			playerState->otherFlags = 0;
			iPrintLn(m_ClientNum, "God Mode ^1Off");
		}
	}

	void Menu::ToggleAmmo()
	{
		DWORD address = 0x8235BB54;
		unsigned int defaultValue = 0x7D3D5050;
		unsigned int modifiedValue = 0x7D495378;

		if (Memory::Read<unsigned int>(address) == defaultValue)
		{
			Memory::Write<unsigned int>(address, modifiedValue);
			iPrintLn(m_ClientNum, "Unlimited Ammo ^2On");
		}
		else
		{
			Memory::Write<unsigned int>(address, defaultValue);
			iPrintLn(m_ClientNum, "Unlimited Ammo ^1Off");
		}
	}

	void Menu::ToggleUFO()
	{
		if (GetGClient(m_ClientNum)->mFlags != 2)
		{
			GetGClient(m_ClientNum)->mFlags = 2;
			iPrintLn(m_ClientNum, "Ufo ^2On");
		}
		else
		{
			GetGClient(m_ClientNum)->mFlags = 0;
			iPrintLn(m_ClientNum, "Ufo ^1Off");
		}
	}

	void Menu::SavePosition()
	{
		m_SavedPos = SV_GetPlayerstateForClientNum(m_ClientNum)->origin;
		m_SavedAngles = SV_GetPlayerstateForClientNum(m_ClientNum)->viewAngles;

		iPrintLn(m_ClientNum, "Position ^2Saved");
	}

	void Menu::LoadPosition()
	{
		if (m_SavedPos == vec3(0.0f, 0.0f, 0.0f) || m_SavedAngles == vec3(0.0f, 0.0f, 0.0f))
		{
			iPrintLn(m_ClientNum, "^1Save a position first!");
			return;
		}

		float origin[] = { m_SavedPos.x, m_SavedPos.y, m_SavedPos.z };
		float angles[] = { m_SavedAngles.x, m_SavedAngles.y, m_SavedAngles.z };

		TeleportPlayer(GetEntity(m_ClientNum), origin, angles);
	}

	void Menu::ToggleSaveLoadBinds()
	{
		if (!m_BindsEnabled)
			iPrintLn(m_ClientNum, "Press RB to ^2Save^7 and LB to ^2Load");
		else
			iPrintLn(m_ClientNum, "Save and Load binds ^1Off");

		m_BindsEnabled = !m_BindsEnabled;
	}

	void Menu::CreateStructure()
	{
		m_Structure["Cod Jumper"] = std::vector<Option>();
		m_Structure["Cod Jumper"].reserve(2);
		m_Structure["Cod Jumper"].emplace_back(Option("Main", 0));
		m_Structure["Cod Jumper"].emplace_back(Option("Teleport", 1));

		m_Structure["Main"] = std::vector<Option>();
		m_Structure["Main"].reserve(2);
		m_Structure["Main"].emplace_back(Option("God Mode", 0));
		m_Structure["Main"].emplace_back(Option("Ammo", 1));

		m_Structure["Teleport"] = std::vector<Option>();
		m_Structure["Teleport"].reserve(4);
		m_Structure["Teleport"].emplace_back(Option("Save/Load Binds", 0));
		m_Structure["Teleport"].emplace_back(Option("Save Position", 1));
		m_Structure["Teleport"].emplace_back(Option("Load Position", 2));
		m_Structure["Teleport"].emplace_back(Option("UFO", 3));
	}

	void Menu::OnSelectPressed(const std::string& optionName)
	{
		if (optionName == "Main" || optionName == "Teleport")
			GoToMenu(optionName);
		else if (optionName == "God Mode")
			ToggleGodMode();
		else if (optionName == "Ammo")
			ToggleAmmo();
		else if (optionName == "UFO")
			ToggleUFO();
		else if (optionName == "Save Position")
			SavePosition();
		else if (optionName == "Load Position")
			LoadPosition();
		else if (optionName == "Save/Load Binds")
			ToggleSaveLoadBinds();
	}

	void Menu::OnBackPressed(const std::string& optionName)
	{
		if (optionName == "Main" || optionName == "Teleport")
			return;

		GoToMenu("Cod Jumper");
	}

	void Menu::GoToMenu(const std::string& menuName)
	{
		ResetCursor();

		m_Title.SetText(menuName);
	}

	void Menu::Update()
	{
		if (!m_Open)
			return;

		m_Background.Draw();
		m_Title.Draw();
		m_Scroller.Draw();
		m_Instructions.Draw();
		
		for (size_t i = 0; i < m_Structure[m_Title.GetText()].size(); i++)
			m_Structure[m_Title.GetText()][i].Draw();

	}

	void Menu::OnEvent(const std::string& eventString)
	{
		if (eventString == DPAD_LEFT)
			m_Open = !m_Open;

		if (eventString == DPAD_UP && m_Open)
		{
			m_CurrentScrollerPos--;

			if (m_CurrentScrollerPos < 0)
				m_CurrentScrollerPos = m_Structure[m_Title.GetText()].size() - 1;

			MoveScroller(m_CurrentScrollerPos);
		}

		if (eventString == DPAD_DOWN && m_Open)
		{
			m_CurrentScrollerPos++;

			if (m_CurrentScrollerPos >= (int)m_Structure[m_Title.GetText()].size())
				m_CurrentScrollerPos = 0;

			MoveScroller(m_CurrentScrollerPos);
		}

		if (eventString == SELECT && m_Open)
			OnSelectPressed(m_Structure[m_Title.GetText()][m_CurrentScrollerPos].GetText());
		
		if (eventString == BACK && m_Open)
			OnBackPressed(m_Structure[m_Title.GetText()][m_CurrentScrollerPos].GetText());

		if (eventString == LB && m_BindsEnabled)
			LoadPosition();

		if (eventString == RB && m_BindsEnabled)
			SavePosition();
	}

	void Menu::MoveScroller(int position)
	{
		m_Scroller.SetY(s_MenuY + s_Padding * 2 + s_TitleHeight + s_LineHeight * position);
	}

	void Menu::ResetCursor()
	{
		m_CurrentScrollerPos = 0;
		MoveScroller(m_CurrentScrollerPos);
	}
}
}