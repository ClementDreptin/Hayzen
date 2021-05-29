#include "pch.h"
#include "Games\SpecOps\MW3\Menu.h"

#include "Games\SpecOps\MW3\Functions.h"
#include "Games\SpecOps\MW3\MW3.h"
#include "Games\SpecOps\MW3\Events.h"

using namespace XexUtils;

namespace SpecOps
{
namespace MW3
{
	std::unordered_map<std::string, std::vector<std::string>> Menu::s_Structure;

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
		m_Background = RectangleElem(s_MenuX, s_MenuY, s_MenuWidth, s_MenuHeight, HudElem::s_ColorBlackNoAlpha);

		m_Title = TextElem("Cod Jumper", s_MenuX + s_Padding, s_Padding + s_TitleHeight, HudElem::s_ColorWhiteNoAlpha, 1.7f);

		m_Scroller = RectangleElem(s_MenuX, s_MenuY + (s_Padding * 2) + s_TitleHeight, s_MenuWidth, s_LineHeight, HudElem::s_ColorWhiteNoAlpha);

		m_Instructions = TextElem("Navigate: UP - DOWN | Select: X | Back: RS",
			s_MenuX + s_Padding, s_MenuY + s_MenuHeight - s_Padding - 80, HudElem::s_ColorWhiteNoAlpha, 0.7f);

		CreateStructure();

		RequestMenuChange("Cod Jumper");
	}

	void Menu::ToggleFallDamage()
	{
		DWORD address = 0x82000D68;

		if (Memory::Read<float>(address) == 128.0f)
		{
			Memory::Write<float>(address, 9999.0f);
			iPrintLn(m_ClientNum, "Fall Damage ^2Off");
		}
		else
		{
			Memory::Write<float>(address, 128.0f);
			iPrintLn(m_ClientNum, "Fall Damage ^1On");
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

	void Menu::ToggleOldSchool()
	{
		DWORD address = 0x8202E160;

		if (Memory::Read<float>(address) == 39.0f)
		{
			Memory::Write<float>(address, 64.0f);
			iPrintLn(m_ClientNum, "Old School ^2On");
		}
		else
		{
			Memory::Write<float>(address, 39.0f);
			iPrintLn(m_ClientNum, "Old School ^1Off");
		}
	}

	void Menu::ToggleBlastMarks()
	{
		if (Dvar_GetBool("fx_marks"))
		{
			SetClientDvar(m_ClientNum, "fx_marks", "0");
			iPrintLn(m_ClientNum, "Blast Marks ^2Off");
		}
		else
		{
			SetClientDvar(m_ClientNum, "fx_marks", "1");
			iPrintLn(m_ClientNum, "Blast Marks ^1On");
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
			iPrintLn(m_ClientNum, "Press [{+frag}] to ^2Save^7 and [{+smoke}] to ^2Load");
		else
			iPrintLn(m_ClientNum, "Save and Load binds ^1Off");

		m_BindsEnabled = !m_BindsEnabled;
	}

	void Menu::CreateStructure()
	{
		s_Structure["Cod Jumper"] = std::vector<std::string>();
		s_Structure["Cod Jumper"].reserve(3);
		s_Structure["Cod Jumper"].emplace_back("Main");
		s_Structure["Cod Jumper"].emplace_back("Teleport");
		s_Structure["Cod Jumper"].emplace_back("Admin");

		s_Structure["Main"] = std::vector<std::string>();
		s_Structure["Main"].reserve(5);
		s_Structure["Main"].emplace_back("Fall Damage");
		s_Structure["Main"].emplace_back("Ammo");
		s_Structure["Main"].emplace_back("Blast Marks");
		s_Structure["Main"].emplace_back("Old School");
		s_Structure["Main"].emplace_back("Depatch Bounces");

		s_Structure["Teleport"] = std::vector<std::string>();
		s_Structure["Teleport"].reserve(4);
		s_Structure["Teleport"].emplace_back("Save/Load Binds");
		s_Structure["Teleport"].emplace_back("Save Position");
		s_Structure["Teleport"].emplace_back("Load Position");
		s_Structure["Teleport"].emplace_back("UFO");

		s_Structure["Admin"] = std::vector<std::string>();
		s_Structure["Admin"].reserve(2);
		s_Structure["Admin"].emplace_back("Infect");
			s_Structure["Infect"] = std::vector<std::string>();
			s_Structure["Infect"].reserve(1);
			s_Structure["Infect"].emplace_back("Knockback");
	}

	void Menu::OnSelectPressed(const std::string& optionName)
	{
		if (optionName == "Main" || optionName == "Teleport" || optionName == "Admin" || optionName == "Infect")
			RequestMenuChange(optionName);
		else if (optionName == "Fall Damage")
			ToggleFallDamage();
		else if (optionName == "Ammo")
			ToggleAmmo();
		else if (optionName == "Old School")
			ToggleOldSchool();
		else if (optionName == "Blast Marks")
			ToggleBlastMarks();
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
		if (optionName == "Main" || optionName == "Teleport" || optionName == "Admin")
			return;

		RequestMenuChange("Cod Jumper");
	}

	void Menu::Open()
	{
		m_Background.MakeAppear(0.7f);
		m_Title.MakeAppear();
		m_Scroller.MakeAppear(0.7f);
		m_Instructions.MakeAppear();

		for (size_t i = 0; i < m_Options.size(); i++)
			m_Options[i].Activate();

		m_Open = true;
	}

	void Menu::Close()
	{
		m_Background.MakeDisappear();
		m_Title.MakeDisappear();
		m_Scroller.MakeDisappear();
		m_Instructions.MakeDisappear();

		for (size_t i = 0; i < m_Options.size(); i++)
			m_Options[i].Deactivate();

		m_Open = false;
	}

	void Menu::RequestMenuChange(const std::string& menuName)
	{
		m_MenuChangeRequestQueue.push(menuName);
	}

	void Menu::GoToMenu(const std::string& menuName)
	{
		Cleanup();

		if (m_Open) // If m_Open is false, it means this is the call from the constructor and the title has already been set.
			m_Title.SetText(menuName);

		m_Options.reserve(s_Structure[menuName].size());

		for (size_t i = 0; i < s_Structure[menuName].size(); i++)
			m_Options.emplace_back(Option(s_Structure[menuName][i], i, m_Open));
	}

	void Menu::Update()
	{
		m_Background.Draw();
		m_Title.Draw();
		m_Scroller.Draw();
		m_Instructions.Draw();
		
		for (size_t i = 0; i < m_Options.size(); i++)
			m_Options[i].Draw();

		if (!m_MenuChangeRequestQueue.empty())
		{
			std::string& menuName = m_MenuChangeRequestQueue.front();
			GoToMenu(menuName);
			m_MenuChangeRequestQueue.pop();
		}

	}

	void Menu::OnEvent(const std::string& eventString)
	{
		if (eventString == DPAD_LEFT)
		{
			if (!m_Open)
				Open();
			else
				Close();
		}

		if (eventString == DPAD_UP && m_Open)
		{
			m_CurrentScrollerPos--;

			if (m_CurrentScrollerPos < 0)
				m_CurrentScrollerPos = m_Options.size() - 1;

			MoveScroller(m_CurrentScrollerPos);
		}

		if (eventString == DPAD_DOWN && m_Open)
		{
			m_CurrentScrollerPos++;

			if (m_CurrentScrollerPos >= (int)m_Options.size())
				m_CurrentScrollerPos = 0;

			MoveScroller(m_CurrentScrollerPos);
		}

		if (eventString == SELECT && m_Open)
			OnSelectPressed(m_Options[m_CurrentScrollerPos].GetName());

		if (eventString == BACK && m_Open)
			OnBackPressed(m_Options[m_CurrentScrollerPos].GetName());

		if (eventString == LB && m_BindsEnabled)
			LoadPosition();

		if (eventString == RB && m_BindsEnabled)
			SavePosition();
	}

	void Menu::MoveScroller(int position)
	{
		m_Scroller.SetY(s_MenuY + s_Padding * 2 + s_TitleHeight + s_LineHeight * position);
	}

	void Menu::Cleanup()
	{
		m_CurrentScrollerPos = 0;
		MoveScroller(m_CurrentScrollerPos);

		m_Options.clear();
	}
}
}