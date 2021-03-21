#include "pch.h"
#include "Games\Alpha\MW2\Menu.h"

#include "Games\Alpha\MW2\Functions.h"
#include "Utils\Utils.h"
#include "Games\Alpha\MW2\MW2.h"
#include "Utils\Formatter.h"

namespace Alpha
{
namespace MW2
{
	std::unordered_map<std::string, std::vector<std::string>> Menu::s_Structure;

	Menu::Menu(int clientNum)
		: m_ClientNum(clientNum), m_Open(false), m_CurrentScrollerPos(0), m_SavedPos(vec3(0.0f, 0.0f, 0.0f)), m_SavedAngles(vec3(0.0f, 0.0f, 0.0f)), m_BindsEnabled(false)
	{
		m_Background = HudElem_Alloc(clientNum, 0);
		SetShader(m_Background, "white", m_MenuX, m_MenuY, m_MenuWidth, m_MenuHeight, COLOR_BLACK_NO_ALPHA);

		m_Title = HudElem_Alloc(clientNum, 0);
		SetText(m_Title, "Cod Jumper", 3.0f, m_MenuX + m_MenuWidth / 2, m_MenuY + m_Padding, COLOR_WHITE_NO_ALPHA);

		m_Scroller = HudElem_Alloc(clientNum, 0);
		SetShader(m_Scroller, "white", m_MenuX, m_MenuY + (m_Padding * 2) + m_TitleHeight, m_MenuWidth, m_LineHeight, COLOR_WHITE_NO_ALPHA, 0.5f);

		CreateStructure();

		GoToMenu("Cod Jumper");
	}

	void Menu::Knockback()
	{
		XOVERLAPPED Overlapped;
		wchar_t wideBuffer[7];
		char buffer[7];
		ZeroMemory(&Overlapped, sizeof(Overlapped));
		XShowKeyboardUI(0, VKBD_LATIN_NUMERIC, L"30000", L"Knockback", L"Recommended value: 30000", wideBuffer, 7, &Overlapped);

		while (!XHasOverlappedIoCompleted(&Overlapped))
			Sleep(100);

		wcstombs(buffer, wideBuffer, 7);

		if (!strcmp(buffer, ""))
			return;

		SetClientDvar(-1, "g_knockback", buffer);

		iPrintLn(-1, "Knockback set to ^2" + std::string(buffer));
	}

	void Menu::ToggleDepatchBounces()
	{
		DWORD branchAddress = 0x8210CBDC;
		unsigned int defaultValue = 0x409AFFB0;
		unsigned int modifiedValue = 0x6060FFB0;

		if (Utils::Read<unsigned int>(branchAddress) == defaultValue)
		{
			Utils::Write<unsigned int>(branchAddress, modifiedValue);
			iPrintLn(m_ClientNum, "Depatch Bounces ^2On");
		}
		else
		{
			Utils::Write<unsigned int>(branchAddress, defaultValue);
			iPrintLn(m_ClientNum, "Depatch Bounces ^1Off");
		}
	}

	void Menu::ToggleFallDamage()
	{
		if (Dvar_GetFloat("bg_fallDamageMinHeight") == 128.0f)
		{
			SetClientDvar(-1, "bg_fallDamageMinHeight", "9998");
			SetClientDvar(-1, "bg_fallDamageMaxHeight", "9999");
			iPrintLn(m_ClientNum, "Fall Damage ^2Off");
		}
		else
		{
			SetClientDvar(-1, "bg_fallDamageMinHeight", "128");
			SetClientDvar(-1, "bg_fallDamageMaxHeight", "300");
			iPrintLn(m_ClientNum, "Fall Damage ^1On");
		}
	}

	void Menu::ToggleAmmo()
	{
		if (!Dvar_GetBool("player_sustainAmmo"))
		{
			SetClientDvar(-1, "player_sustainAmmo", "1");
			iPrintLn(m_ClientNum, "Unlimited Ammo ^2On");
		}
		else
		{
			SetClientDvar(-1, "player_sustainAmmo", "0");
			iPrintLn(m_ClientNum, "Unlimited Ammo ^1Off");
		}
	}

	void Menu::ToggleOldSchool()
	{
		if (Dvar_GetFloat("jump_height") != 64.0f)
		{
			SetClientDvar(-1, "jump_height", "64");
			SetClientDvar(-1, "jump_slowdownEnable", "0");
			iPrintLn(m_ClientNum, "Old School ^2On");
		}
		else
		{
			SetClientDvar(-1, "jump_height", "39");
			SetClientDvar(-1, "jump_slowdownEnable", "1");
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
		Cbuf_AddText(m_ClientNum, "ufo");
	}

	void Menu::SavePosition()
	{
		m_SavedPos = GetPlayerState(m_ClientNum)->origin;
		m_SavedAngles = GetPlayerState(m_ClientNum)->viewAngles;

		iPrintLn(m_ClientNum, "Position ^2Saved");
	}

	void Menu::LoadPosition()
	{
		if (m_SavedPos == vec3(0.0f, 0.0f, 0.0f) || m_SavedAngles == vec3(0.0f, 0.0f, 0.0f))
		{
			iPrintLn(m_ClientNum, "^1Save a position first!");
			return;
		}
		
		Cbuf_AddText(m_ClientNum, Formatter::Format("setviewpos %i %i %i %i %i", (int)m_SavedPos.x, (int)m_SavedPos.y, (int)m_SavedPos.z, (int)m_SavedAngles.x, (int)m_SavedAngles.y).c_str());
		
		iPrintLn(m_ClientNum, "Position ^2Loaded");
	}

	void Menu::ToggleSaveLoadBinds()
	{
		if (!m_BindsEnabled)
			iPrintLn(m_ClientNum, "Press [{+frag}] to ^2Save^7 and [{+smoke}] to ^2Load");
		else
			iPrintLn(m_ClientNum, "Save and Load binds ^1Off");

		m_BindsEnabled = !m_BindsEnabled;
	}

	void Menu::ToggleGodMode()
	{
		int GOD_MODE_ON = 4097;
		int GOD_MODE_OFF = 4096;

		if (GetPlayerEntity(m_ClientNum)->flags == GOD_MODE_OFF)
		{
			GetPlayerEntity(m_ClientNum)->flags = GOD_MODE_ON;
			iPrintLn(m_ClientNum, "God Mode ^2On");
		}
		else
		{
			GetPlayerEntity(m_ClientNum)->flags = GOD_MODE_OFF;
			iPrintLn(m_ClientNum, "God Mode ^1Off");
		}
	}

	void Menu::Verify(int clientNum)
	{
		if (MW2::Verify(clientNum))
			iPrintLn(clientNum, "You have been ^2Verified^7, press [{+actionslot 3}] to ^2Open");
		else
			iPrintLn(m_ClientNum, "^1This player is already Verified!");
	}

	void Menu::SpawnCP()
	{
		gentity_s* currentMapEntity = GetCurrentMapEntity();
		if (!currentMapEntity)
		{
			iPrintLn(m_ClientNum, "^1You cannot spawn a Care Package on this map!");
			return;
		}

		float distance = 100.0f;

		vec3 origin = GetPlayerState(m_ClientNum)->origin;
		float viewY = GetPlayerState(m_ClientNum)->viewAngles.y;

		vec3 cratePos;
		cratePos.x = origin.x + (float)(distance * cos(Utils::Radians(viewY)));
		cratePos.y = origin.y + (float)(distance * sin(Utils::Radians(viewY)));
		cratePos.z = origin.z;

		gentity_s* entity = G_Spawn();
		entity->r.currentOrigin = cratePos;
		entity->r.currentAngles.y = viewY;

		G_SetModel(entity, "com_plasticcase_friendly");
		SP_script_model(entity);
		SV_UnlinkEntity(entity);
		entity->r.bmodel = 4;
		entity->state.index = currentMapEntity->state.index;
		
		int contents = entity->r.contents;
		SV_SetBrushModel(entity);
		contents |= entity->r.contents;
		entity->r.contents = contents;

		SV_LinkEntity(entity);
	}

	void Menu::CreateStructure()
	{
		s_Structure["Cod Jumper"] = std::vector<std::string>();
		s_Structure["Cod Jumper"].reserve(3);
		s_Structure["Cod Jumper"].emplace_back("Main");
		s_Structure["Cod Jumper"].emplace_back("Teleport");
		s_Structure["Cod Jumper"].emplace_back("Admin");

		s_Structure["Main"] = std::vector<std::string>();
		s_Structure["Main"].reserve(7);
		s_Structure["Main"].emplace_back("God Mode");
		s_Structure["Main"].emplace_back("Fall Damage");
		s_Structure["Main"].emplace_back("Ammo");
		s_Structure["Main"].emplace_back("Blast Marks");
		s_Structure["Main"].emplace_back("Old School");
		s_Structure["Main"].emplace_back("Depatch Bounces");
		s_Structure["Main"].emplace_back("Spawn Care Package");

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
		s_Structure["Admin"].emplace_back("Verify");
			s_Structure["Verify"] = std::vector<std::string>();
	}

	void Menu::OnAPressed(const std::string& optionName)
	{
		int pos;

		if (optionName == "Main" || optionName == "Teleport" || optionName == "Admin" || optionName == "Infect")
			GoToMenu(optionName);
		else if (optionName == "Verify")
		{
			GetAllPlayers();
			GoToMenu(optionName);
		}
		else if ((pos = optionName.find("(")) != std::string::npos)
			Verify(std::stoi(optionName.substr(pos + 1, 1)));
		else if (optionName == "Knockback")
			_Knockback();
		else if (optionName == "Depatch Bounces")
			ToggleDepatchBounces();
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
		else if (optionName == "God Mode")
			ToggleGodMode();
		else if (optionName == "Spawn Care Package")
			SpawnCP();
		else
			ToDo();
	}

	void Menu::OnBPressed(const std::string& optionName)
	{
		if (optionName == "Main" || optionName == "Teleport" || optionName == "Admin")
			return;

		if (optionName == "Knockback" || optionName.find("(") != std::string::npos)
		{
			GoToMenu("Admin");
			return;
		}

		GoToMenu("Cod Jumper");
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

	void Menu::GoToMenu(const std::string& menuName)
	{
		Cleanup();

		if (m_Open) // If m_Open is false, it means this is the call from the constructor and the title has already been set.
			SetText(m_Title, menuName.c_str(), m_Title->elem.fontScale, (int)m_Title->elem.x, (int)m_Title->elem.y);

		m_Options.reserve(s_Structure[menuName].size());

		for (size_t i = 0; i < s_Structure[menuName].size(); i++)
			m_Options.emplace_back(Option(m_ClientNum, s_Structure[menuName][i], i, m_Open));
	}

	void Menu::GetAllPlayers()
	{
		s_Structure["Verify"].clear();

		for (int i = 0; i < 18; i++)
		{
			if (!strcmp(GetClientState(i)->name, ""))
				continue;

			s_Structure["Verify"].emplace_back(std::string(GetClientState(i)->name) + " (" + std::to_string((long long)i) + ")");
		}
	}

	DWORD Menu::StaticKnockbackThread(LPVOID lpThreadParameter)
	{
		Menu* This = (Menu*)lpThreadParameter;
		This->Knockback();
		return 0;
	}

	void Menu::_Knockback()
	{
		Utils::Thread((LPTHREAD_START_ROUTINE)StaticKnockbackThread, (void*)this);
	}

	void Menu::ToDo()
	{
		iPrintLn(m_ClientNum, "^1Not Implemented Yet!");
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

		if (eventString == "LB" && m_BindsEnabled)
			LoadPosition();

		if (eventString == "RB" && m_BindsEnabled)
			SavePosition();
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
}