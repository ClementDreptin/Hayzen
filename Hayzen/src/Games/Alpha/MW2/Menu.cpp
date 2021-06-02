#include "pch.h"
#include "Games\Alpha\MW2\Menu.h"

#include "Games\Alpha\MW2\Functions.h"
#include "Games\Alpha\MW2\MW2.h"
#include "Games\Alpha\MW2\Events.h"

namespace Alpha
{
namespace MW2
{
	gentity_s* Menu::s_Bot;

	Menu::Menu(int clientNum)
		: m_ClientNum(clientNum), m_Open(false), m_CurrentScrollerPos(0), m_SavedPos(vec3(0.0f, 0.0f, 0.0f)), m_SavedAngles(vec3(0.0f, 0.0f, 0.0f)), m_BindsEnabled(false)
	{
		m_Background = RectangleElem(HudElem::s_MenuX, HudElem::s_MenuY, HudElem::s_MenuWidth, HudElem::s_MenuHeight, HudElem::s_ColorBlack);
		m_Background.SetAlpha(0.7f);

		m_Title = TextElem("Cod Jumper", HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_Padding + HudElem::s_TitleHeight, HudElem::s_ColorWhite, 1.7f);

		m_Scroller = RectangleElem(HudElem::s_MenuX, HudElem::s_MenuY + (HudElem::s_Padding * 2) + HudElem::s_TitleHeight, HudElem::s_MenuWidth, HudElem::s_LineHeight, HudElem::s_ColorWhite);
		m_Scroller.SetAlpha(0.7f);

		m_Instructions = TextElem("Navigate: UP - DOWN | Select: X | Back: RS",
			HudElem::s_MenuX + HudElem::s_Padding, HudElem::s_MenuY + HudElem::s_MenuHeight - HudElem::s_Padding - 80, HudElem::s_ColorWhite, 0.7f);

		CreateStructure();
	}

	void Menu::ToggleDepatchBounces()
	{
		DWORD branchAddress = 0x8210CBDC;
		unsigned int defaultValue = 0x409AFFB0;
		unsigned int modifiedValue = 0x6060FFB0;

		if (Memory::Read<unsigned int>(branchAddress) == defaultValue)
		{
			Memory::Write<unsigned int>(branchAddress, modifiedValue);
			iPrintLn(m_ClientNum, "Depatch Bounces ^2On");
		}
		else
		{
			Memory::Write<unsigned int>(branchAddress, defaultValue);
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

	void Menu::ToggleGodMode()
	{
		int GOD_MODE_ON = 4097;
		int GOD_MODE_OFF = 4096;

		if (GetEntity(m_ClientNum)->flags == GOD_MODE_OFF)
		{
			GetEntity(m_ClientNum)->flags = GOD_MODE_ON;
			iPrintLn(m_ClientNum, "God Mode ^2On");
		}
		else
		{
			GetEntity(m_ClientNum)->flags = GOD_MODE_OFF;
			iPrintLn(m_ClientNum, "God Mode ^1Off");
		}
	}

	void Menu::SpawnCP()
	{
		gentity_s* currentMapBrushModel = GetCurrentMapBrushModel();
		if (!currentMapBrushModel)
		{
			iPrintLn(m_ClientNum, "^1You cannot spawn a Care Package on this map!");
			return;
		}

		float distance = 150.0f;
		vec3 origin = GetPlayerState(m_ClientNum)->origin;
		float viewY = GetPlayerState(m_ClientNum)->viewAngles.y;

		gentity_s* entity = G_Spawn();
		entity->r.currentOrigin = Math::ToFront(origin, viewY, distance);
		entity->r.currentAngles.y = viewY;

		G_SetModel(entity, "com_plasticcase_friendly");
		SP_script_model(entity);
		SV_UnlinkEntity(entity);
		entity->r.bmodel = 4;
		entity->state.index = currentMapBrushModel->state.index;
		
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
		Memory::Thread((LPTHREAD_START_ROUTINE)StaticSpawnBotThread, (void*)this);
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

		s_Bot->client->ps.origin = Math::ToFront(origin, viewY, distance);
	}

	void Menu::ToggleBotMovement()
	{
		if (!s_Bot)
		{
			iPrintLn(m_ClientNum, "^1There is no bot in the game!");
			return;
		}

		if (Dvar_GetBool("testClients_doMove"))
		{
			SetClientDvar(-1, "testClients_doMove", "0");
			m_Structure[m_Title.GetText()][m_CurrentScrollerPos].SetText("Unfreeze Bot");
		}
		else
		{
			SetClientDvar(-1, "testClients_doMove", "1");
			m_Structure[m_Title.GetText()][m_CurrentScrollerPos].SetText("Freeze Bot");
		}
	}

	void Menu::CreateStructure()
	{
		m_Structure["Cod Jumper"] = std::vector<Option>();
		m_Structure["Cod Jumper"].reserve(3);
		m_Structure["Cod Jumper"].emplace_back(Option("Main", 0));
		m_Structure["Cod Jumper"].emplace_back(Option("Teleport", 1));
		m_Structure["Cod Jumper"].emplace_back(Option("Bot", 2));

		m_Structure["Main"] = std::vector<Option>();
		m_Structure["Main"].reserve(5);
		m_Structure["Main"].emplace_back(Option("God Mode", 0));
		m_Structure["Main"].emplace_back(Option("Fall Damage", 1));
		m_Structure["Main"].emplace_back(Option("Ammo", 2));
		m_Structure["Main"].emplace_back(Option("Depatch Bounces", 3));
		m_Structure["Main"].emplace_back(Option("Spawn Care Package", 4));

		m_Structure["Teleport"] = std::vector<Option>();
		m_Structure["Teleport"].reserve(4);
		m_Structure["Teleport"].emplace_back(Option("Save/Load Binds", 0));
		m_Structure["Teleport"].emplace_back(Option("Save Position", 1));
		m_Structure["Teleport"].emplace_back(Option("Load Position", 2));
		m_Structure["Teleport"].emplace_back(Option("UFO", 3));

		m_Structure["Bot"] = std::vector<Option>();
		m_Structure["Bot"].reserve(3);
		m_Structure["Bot"].emplace_back(Option("Spawn Bot", 0));
		m_Structure["Bot"].emplace_back(Option("Teleport Bot To Me", 1));
		m_Structure["Bot"].emplace_back(Option("Unfreeze Bot", 2));
	}

	void Menu::OnSelectPressed(const std::string& optionName)
	{
		if (optionName == "Main" || optionName == "Teleport" || optionName == "Bot")
			GoToMenu(optionName);
		else if (optionName == "Depatch Bounces")
			ToggleDepatchBounces();
		else if (optionName == "Fall Damage")
			ToggleFallDamage();
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
		else if (optionName == "God Mode")
			ToggleGodMode();
		else if (optionName == "Spawn Care Package")
			SpawnCP();
		else if (optionName == "Spawn Bot")
			SpawnBot();
		else if (optionName == "Teleport Bot To Me")
			TeleportBotToMe();
		else if (optionName == "Unfreeze Bot" || optionName == "Freeze Bot")
			ToggleBotMovement();
	}

	void Menu::OnBackPressed(const std::string& optionName)
	{
		if (optionName == "Main" || optionName == "Teleport" || optionName == "Bot")
			return;

		GoToMenu("Cod Jumper");
	}

	void Menu::GoToMenu(const std::string& menuName)
	{
		ResetScroller();

		m_Title.SetText(menuName);
	}

	DWORD Menu::StaticSpawnBotThread(LPVOID lpThreadParameter)
	{
		Menu* This = (Menu*)lpThreadParameter;
		int serverId = Memory::Read<int>(0x8355D5C4);
		std::string chooseTeamCmd = Formatter::Format("mr %i 4 autoassign", serverId);
		std::string chooseClassCmd = Formatter::Format("mr %i 11 class0", serverId);
		int botPtr = Memory::Read<int>(0x83577D98) + s_Bot->state.number * 0x97F80;

		Sleep(150);
		
		SV_ExecuteClientCommand(botPtr, chooseTeamCmd.c_str(), 1, 0);
		Sleep(150);
		
		SV_ExecuteClientCommand(botPtr, chooseClassCmd.c_str(), 1, 0);
		Sleep(150);
		
		SetClientDvar(-1, "testClients_doMove", "0");
		SetClientDvar(-1, "testClients_doAttack", "0");
		SetClientDvar(-1, "testClients_watchKillcam", "0");

		This->TeleportBotToMe();

		return 0;
	}

	void Menu::FreeBot()
	{
		s_Bot = nullptr;
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
		m_Scroller.SetY(HudElem::s_MenuY + HudElem::s_Padding * 2 + HudElem::s_TitleHeight + HudElem::s_LineHeight * position);
	}

	void Menu::ResetScroller()
	{
		m_CurrentScrollerPos = 0;
		MoveScroller(m_CurrentScrollerPos);
	}
}
}