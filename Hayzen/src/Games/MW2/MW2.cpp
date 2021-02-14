#include "pch.h"
#include "Games\MW2\MW2.h"

#include "Utils\Utils.h"

// TEMP
#include "MenuFunctions.h"

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

		// Allow walking in game timer and when game has ended
		*(int*)(0x821D6DC8) = 0x60000000;

		// TODO: figure out what is wrong with the hooks
		//Utils::PatchInJump((DWORD*)0x823B64C4, (DWORD)XamInputGetStateHook, 0); --> entirely freezes the console
		//Utils::PatchInJump((DWORD*)0x82253140, (DWORD)SV_ExecuteClientCommandHook, 0); --> corrupts the select save device pop-up
	}
}

DWORD MW2::XamInputGetStateHook(DWORD dwUserIndex, PXINPUT_STATE pState)
{
	if (Game.inGame())
		if (!Game.gameStarted)
			Game.StartupGame();
	else
		if (Game.gameStarted)
			Game.ResetGame();

	return XInputGetState(dwUserIndex, pState);
}

void MW2::SV_ExecuteClientCommandHook(unsigned long client, const char * s, bool clientOK)
{
	DWORD clientIndex = (client - *(int*)0x83623B98) / 0x97F80;
	SV_Cmd_TokenizeString(s);
	ClientCommand(clientIndex);
	SV_Cmd_EndTokenizedString();
	MonitorConnectionState(clientIndex, s);
}