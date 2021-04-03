#include "pch.h"
#include "Games\MW2\Menu.h"

#include "Games\MW2\Functions.h"
#include "Games\MW2\MW2.h"

namespace MW2
{
	std::unordered_map<std::string, std::vector<std::string>> Menu::s_Structure;
	gentity_s* Menu::s_Bot;

	Menu::Menu(int clientNum)
		: m_ClientNum(clientNum), m_Open(false), m_CurrentScrollerPos(0), m_SavedPos(vec3(0.0f, 0.0f, 0.0f)), m_BindsEnabled(false)
	{
		m_Background = HudElem_Alloc(clientNum, 0);
		SetShader(m_Background, "white", m_MenuX, m_MenuY, m_MenuWidth, m_MenuHeight, COLOR_BLACK_NO_ALPHA);

		m_Title = HudElem_Alloc(clientNum, 0);
		SetText(m_Title, "Cod Jumper", 3.0f, m_MenuX + m_MenuWidth / 2, m_MenuY + m_Padding, COLOR_WHITE_NO_ALPHA);

		m_Scroller = HudElem_Alloc(clientNum, 0);
		SetShader(m_Scroller, "white", m_MenuX, m_MenuY + (m_Padding * 2) + m_TitleHeight, m_MenuWidth, m_LineHeight, COLOR_WHITE_NO_ALPHA, 0.5f);

		m_Instructions = HudElem_Alloc(clientNum, 0);
		SetText(m_Instructions,
				"Navigate: [{+actionslot 1}] - [{+actionslot 2}] | Select: [{+usereload}] | Back: [{+melee}]",
				1.5f,
				m_MenuX + m_MenuWidth / 2, m_MenuY + m_MenuHeight - m_Padding - 80, COLOR_WHITE_NO_ALPHA);

		CreateStructure();

		GoToMenu("Cod Jumper");
	}

	void Menu::ToggleElevators()
	{
		DWORD branchAddress = 0x820D8360;
		unsigned short defaultValue = 0x419A;
		unsigned short modifiedValue = 0x4800;

		if (XexUtils::Memory::Read<unsigned short>(branchAddress) == defaultValue)
		{
			XexUtils::Memory::Write<unsigned short>(branchAddress, modifiedValue);
			iPrintLn(m_ClientNum, "Elevators ^2On");
		}
		else
		{
			XexUtils::Memory::Write<unsigned short>(branchAddress, defaultValue);
			iPrintLn(m_ClientNum, "Elevators ^1Off");
		}
	}

	void Menu::Knockback()
	{
		std::string value = XexUtils::Xam::ShowKeyboard("Knockback", "Recommended value: 30000", "30000", 6, VKBD_LATIN_NUMERIC);

		if (value == "")
			value = "1000";

		SetClientDvar(-1, "g_knockback", value);

		iPrintLn(-1, "Knockback set to ^2" + value);
	}

	void Menu::ToggleDepatchBounces()
	{
		DWORD branchAddress = 0x820DABE4;
		unsigned int defaultValue = 0x409AFFB0;
		unsigned int modifiedValue = 0x6060FFB0;

		if (XexUtils::Memory::Read<unsigned int>(branchAddress) == defaultValue)
		{
			XexUtils::Memory::Write<unsigned int>(branchAddress, modifiedValue);
			iPrintLn(m_ClientNum, "Depatch Bounces ^2On");
		}
		else
		{
			XexUtils::Memory::Write<unsigned int>(branchAddress, defaultValue);
			iPrintLn(m_ClientNum, "Depatch Bounces ^1Off");
		}
	}

	void Menu::ToggleFallDamage()
	{
		DWORD address = 0x82019C48;

		if (XexUtils::Memory::Read<float>(address) == 128.0f)
		{
			XexUtils::Memory::Write<float>(address, 9999.0f);
			iPrintLn(m_ClientNum, "Fall Damage ^2Off");
		}
		else
		{
			XexUtils::Memory::Write<float>(address, 128.0f);
			iPrintLn(m_ClientNum, "Fall Damage ^1On");
		}
	}

	void Menu::ToggleAmmo()
	{
		DWORD address = 0x820E1724;
		unsigned int defaultValue = 0x7D1D4850;
		unsigned int modifiedValue = 0x7D284B78;

		if (XexUtils::Memory::Read<unsigned int>(address) == defaultValue)
		{
			XexUtils::Memory::Write<unsigned int>(address, modifiedValue);
			iPrintLn(m_ClientNum, "Unlimited Ammo ^2On");
		}
		else
		{
			XexUtils::Memory::Write<unsigned int>(address, defaultValue);
			iPrintLn(m_ClientNum, "Unlimited Ammo ^1Off");
		}
	}

	void Menu::ToggleOldSchool()
	{
		DWORD address = 0x82001A34;

		if (XexUtils::Memory::Read<float>(address) == 39.0f)
		{
			XexUtils::Memory::Write<float>(address, 64.0f);
			iPrintLn(m_ClientNum, "Old School ^2On");
		}
		else
		{
			XexUtils::Memory::Write<float>(address, 39.0f);
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
		m_SavedPos = GetPlayerState(m_ClientNum)->origin;

		iPrintLn(m_ClientNum, "Position ^2Saved");
	}

	void Menu::LoadPosition()
	{
		if (m_SavedPos == vec3(0.0f, 0.0f, 0.0f))
		{
			iPrintLn(m_ClientNum, "^1Save a position first!");
			return;
		}
		
		GetPlayerState(m_ClientNum)->origin = m_SavedPos;
		
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

		gentity_s* entity = G_Spawn();
		entity->r.currentOrigin = XexUtils::Math::ToFront(origin, viewY, distance);
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

	void Menu::SpawnBot()
	{
		if (s_Bot)
		{
			iPrintLn(m_ClientNum, "^1There is already a bot in the game!");
			return;
		}

		s_Bot = SV_AddTestClient();
		XexUtils::Memory::Thread((LPTHREAD_START_ROUTINE)StaticSpawnBotThread, (void*)this);
	}

	void Menu::TeleportBotToMe()
	{
		if (!s_Bot)
		{
			iPrintLn(m_ClientNum, "^1There is no bot in the game!");
			return;
		}

		float distance = 100.0f;
		vec3 origin = GetPlayerState(m_ClientNum)->origin;
		float viewY = GetPlayerState(m_ClientNum)->viewAngles.y;

		s_Bot->client->ps.origin = XexUtils::Math::ToFront(origin, viewY, distance);
	}

	void Menu::CreateStructure()
	{
		s_Structure["Cod Jumper"] = std::vector<std::string>();
		s_Structure["Cod Jumper"].reserve(3);
		s_Structure["Cod Jumper"].emplace_back("Main");
		s_Structure["Cod Jumper"].emplace_back("Teleport");
		s_Structure["Cod Jumper"].emplace_back("Admin");

		s_Structure["Main"] = std::vector<std::string>();
		s_Structure["Main"].reserve(9);
		s_Structure["Main"].emplace_back("God Mode");
		s_Structure["Main"].emplace_back("Fall Damage");
		s_Structure["Main"].emplace_back("Ammo");
		s_Structure["Main"].emplace_back("Blast Marks");
		s_Structure["Main"].emplace_back("Old School");
		s_Structure["Main"].emplace_back("Elevators");
		s_Structure["Main"].emplace_back("Depatch Bounces");
		s_Structure["Main"].emplace_back("Spawn Care Package");
		s_Structure["Main"].emplace_back("Spawn Bot");

		s_Structure["Teleport"] = std::vector<std::string>();
		s_Structure["Teleport"].reserve(5);
		s_Structure["Teleport"].emplace_back("Save/Load Binds");
		s_Structure["Teleport"].emplace_back("Save Position");
		s_Structure["Teleport"].emplace_back("Load Position");
		s_Structure["Teleport"].emplace_back("UFO");
		s_Structure["Teleport"].emplace_back("Teleport Bot To Me");

		s_Structure["Admin"] = std::vector<std::string>();
		s_Structure["Admin"].reserve(2);
		s_Structure["Admin"].emplace_back("Infect");
			s_Structure["Infect"] = std::vector<std::string>();
			s_Structure["Infect"].reserve(1);
			s_Structure["Infect"].emplace_back("Knockback");
		s_Structure["Admin"].emplace_back("Verify");
			s_Structure["Verify"] = std::vector<std::string>();
	}

	void Menu::OnSelectPressed(const std::string& optionName)
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
		else if (optionName == "Elevators")
			ToggleElevators();
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
		else if (optionName == "Spawn Bot")
			SpawnBot();
		else if (optionName == "Teleport Bot To Me")
			TeleportBotToMe();
	}

	void Menu::OnBackPressed(const std::string& optionName)
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
		MakeAppear(m_Instructions);

		for (size_t i = 0; i < m_Options.size(); i++)
			m_Options[i].Activate();

		m_Open = true;
	}

	void Menu::Close()
	{
		MakeDisappear(m_Background);
		MakeDisappear(m_Title);
		MakeDisappear(m_Scroller);
		MakeDisappear(m_Instructions);

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

	DWORD Menu::StaticSpawnBotThread(LPVOID lpThreadParameter)
	{
		Menu* This = (Menu*)lpThreadParameter;
		int serverId = XexUtils::Memory::Read<int>(0x8360922C);
		std::string chooseTeamCmd = XexUtils::Formatter::Format("mr %i 3 autoassign", serverId);
		std::string chooseClassCmd = XexUtils::Formatter::Format("mr %i 10 class0", serverId);

		Sleep(150);

		int botPtr = XexUtils::Memory::Read<int>(0x83623B98) + s_Bot->state.number * 0x97F80;

		SV_ExecuteClientCommand(botPtr, chooseTeamCmd.c_str(), 1, 0);
		Sleep(150);

		SV_ExecuteClientCommand(botPtr, chooseClassCmd.c_str(), 1, 0);
		Sleep(150);

		This->TeleportBotToMe();
		return 0;
	}

	void Menu::_Knockback()
	{
		XexUtils::Memory::Thread((LPTHREAD_START_ROUTINE)StaticKnockbackThread, (void*)this);
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

		if (eventString == "select" && m_Open)
			OnSelectPressed(m_Options[m_CurrentScrollerPos].GetName());

		if (eventString == "back" && m_Open)
			OnBackPressed(m_Options[m_CurrentScrollerPos].GetName());

		if (eventString == "LB" && m_BindsEnabled)
			LoadPosition();

		if (eventString == "RB" && m_BindsEnabled)
			SavePosition();
	}

	void Menu::FreeBot()
	{
		s_Bot = nullptr;
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