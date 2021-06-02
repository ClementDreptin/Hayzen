#include "pch.h"
#include "Games\Alpha\MW2\MW2.h"

#include "Games\Alpha\MW2\Events.h"
#include "Games\Alpha\MW2\Functions.h"

namespace Alpha
{
namespace MW2
{
	bool HasGameBegun = false;
	std::unordered_map<int, Client> Clients;

	__declspec(naked) void SCR_DrawScreenFieldStub(const int localClientNum, int refreshedUI)
	{
		__asm
		{
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			li r3, 1
		}
	}

	__declspec(naked) void Scr_NotifyStub(gentity_s* entity, unsigned short stringValue, unsigned int paramCount)
	{
		__asm
		{
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			li r3, 2
		}
	}

	__declspec(naked) void SV_ExecuteClientCommandStub(int client, const char* s, int clientOK, int fromOldServer)
	{
		__asm
		{
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			li r3, 3
		}
	}

	void Init()
	{
		Xam::XNotify("Hayzen - MW2 Alpha Multiplayer Detected");

		Sleep(200);

		// Precache all shaders
		Memory::Write<int>(0x82F3F280 + 0x1C, 1);

		Memory::HookFunctionStart((DWORD*)0x8218B5F0, (DWORD*)SCR_DrawScreenFieldStub, (DWORD)SCR_DrawScreenFieldHook);
		Memory::HookFunctionStart((DWORD*)0x822539C0, (DWORD*)Scr_NotifyStub, (DWORD)Scr_NotifyHook);
		Memory::HookFunctionStart((DWORD*)0x822B4700, (DWORD*)SV_ExecuteClientCommandStub, (DWORD)SV_ExecuteClientCommandHook);
	}

	void SetupGame(int clientNum)
	{
		Verify(clientNum);
		HasGameBegun = true;
	}

	void ResetGame(int clientNum, bool resetBot)
	{
		Clients.erase(clientNum);

		if (resetBot)
			Menu::FreeBot();

		HasGameBegun = false;
	}

	bool Verify(int clientNum)
	{
		if (clientNum < 0 || clientNum > 17)
			return false;

		if (Clients.find(clientNum) != Clients.end() && Clients[clientNum].IsInitialized())
			return false;

		SetClientDvar(clientNum, "loc_warnings", "0");
		SetClientDvar(clientNum, "loc_warningsAsErrors", "0");

		Clients[clientNum] = Client(clientNum);

		return true;
	}

	void SafeReset()
	{
		if (Clients.size() != 0)
			Clients.clear();

		if (HasGameBegun)
		{
			Menu::FreeBot();
			HasGameBegun = false;
		}
	}

	void SCR_DrawScreenFieldHook(const int localClientNum, int refreshedUI)
	{
		SCR_DrawScreenFieldStub(localClientNum, refreshedUI);

		for (size_t i = 0; i < Clients.size(); i++)
			Clients[i].GetMenu().Update();
	}

	void Scr_NotifyHook(gentity_s* entity, unsigned short stringValue, unsigned int paramCount)
	{
		Scr_NotifyStub(entity, stringValue, paramCount);

		int clientNum = entity->state.number;

		const char* notify = SL_ConvertToString(stringValue);

		if (!strcmp(notify, GAME_START))
		{
			// "begin" can happen multiple times a game in round-based gamemodes so we need to reset the menu
			// and recreate it at the beggining of each round if we want the menu to work after the first round
			if (HasGameBegun)
				ResetGame(clientNum, false);

			SetupGame(clientNum);
		}
	}

	void SV_ExecuteClientCommandHook(int client, const char* s, int clientOK, int fromOldServer)
	{
		SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

		int clientNum = (client - Memory::Read<int>(0x83577D98)) / 0x97F80;

		if (HasGameBegun && Clients.find(clientNum) != Clients.end())
			Clients[clientNum].GetMenu().OnEvent(s);

		if (!strcmp(s, "matchdatadone") && Clients.find(clientNum) != Clients.end())
			ResetGame(clientNum);
	}
}
}