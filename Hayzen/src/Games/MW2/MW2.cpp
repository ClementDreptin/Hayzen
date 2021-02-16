#include "pch.h"
#include "Games\MW2\MW2.h"

#include "Utils\Utils.h"

// TEMP
#include "MenuFunctions.h"

__declspec(naked) void RenderSaveStub(const void* args, int unknown)
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
		nop
	}
}

void MW2::Init()
{
	if (!strcmp((char*)0x82001270, "multiplayer"))
	{
		Utils::XNotify("Hayzen - MW2 Multiplayer Detected");

		Sleep(200);

		// Precache all shaders
		*(int*)(0x83109D80 + 0x1C) = 1;

		// NOP cheat protection
		*(int*)(0x8216906C) = 0x60000000;
		*(int*)(0x821690E4) = 0x60000000;

		Utils::HookFunctionStart((DWORD*)0x82285C98, (DWORD*)RenderSaveStub, (DWORD)Menu_PaintAllHook);
		Utils::PatchInJump((DWORD*)0x82253140, (DWORD)SV_ExecuteClientCommandHook, 0);
	}
}

void MW2::Menu_PaintAllHook(const void* args, int unknown)
{
	// TODO:
	// - fix issue removing all HUD (makes the menu not visible)
	if (Game.inGame())
		if (!Game.gameStarted)
			Game.StartupGame();
	else
		if (Game.gameStarted)
			Game.ResetGame();

	RenderSaveStub(args, unknown);
}

void MW2::SV_ExecuteClientCommandHook(unsigned long client, const char * s, bool clientOK)
{
	DWORD clientIndex = (client - *(int*)0x83623B98) / 0x97F80;
	SV_Cmd_TokenizeString(s);
	ClientCommand(clientIndex);
	SV_Cmd_EndTokenizedString();
	MonitorConnectionState(clientIndex, s);
}