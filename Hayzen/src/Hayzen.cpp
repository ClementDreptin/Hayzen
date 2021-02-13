#include "pch.h"
#include "Utils\Xbox.h"
#include "MenuFunctions.h"

DWORD CurrentTitle;

bool DashLoaded, UnloadPlugin;

bool hasBeenOnDash = false;

void SV_ExecuteClientCommandHook(unsigned long client, const char * s, bool clientOK)
{
	DWORD clientIndex = (client - *(int*)0x83623B98) / 0x97F80;
	SV_Cmd_TokenizeString(s);
	ClientCommand(clientIndex);
	SV_Cmd_EndTokenizedString();
	MonitorConnectionState(clientIndex, s);
}

DWORD XamHook(DWORD r3, int r4, PXINPUT_STATE r5)
{
	if (Game.inGame())
	{
		if (!Game.gameStarted)
		{
			printf("Game Has Started\n");
			Game.StartupGame();
		}
	}
	else
		if (Game.gameStarted)
			Game.ResetGame();
	return XInputGetState(r3, r5);
}

enum XBOX_GAMES : DWORD
{
	BOOT_UP = 0x00000000,
	DASHBOARD = 0xFFFE07D1,
	FREESTYLEDASH = 0xF5D20000,
	COD_CLASSIC = 0x584109CA,
	COD_WORLD_AT_WAR = 0x4156081C,
	COD_MODERN_WARFARE = 0x415607E6,
	COD_MODERN_WARFARE_2 = 0x41560817,
	COD_MODERN_WARFARE_3 = 0x415608CB,
	COD_BLACK_OPS_1 = 0x41560855,
	COD_BLACK_OPS_2 = 0x415608C3,
	COD_BLACK_OPS_3 = 0x4156091D,
	COD_GHOSTS = 0x415608FC,
	COD_ADVANCED_WARFARE = 0x41560914,
	CSGO = 0x5841125A,
	DESTINY = 0x415608F8,
	GTAV = 0x545408A7
};

void GameLauncher(DWORD TitleID)
{
	CurrentTitle = TitleID;

	switch (TitleID)
	{
	case DASHBOARD:
		goto Dash;
		break;
	case COD_MODERN_WARFARE_2:
		if (!strcmp((char*)0x82001270, "multiplayer"))
		{
			Xbox::XNotify("Hayzen - MW2 Multiplayer Detected");

			Sleep(200);
			*(int*)(0x83109D80 + 0x1C) = 1;//Precaches Shaders So You Can Use All Different Shaders
			//*(int*)(0x82168FB4) = 0x60000000;//Removes RSA Protection
			*(int*)(0x8216907C) = 0x60000000;//Nops Cheat Protection
			*(int*)(0x821690F4) = 0x60000000;//Nops Cheat Protection
			*(int*)(0x821D6E60) = 0x60000000;//Allows Walking In Game Timer And When Game Has Ended
			PatchInJump((DWORD*)0x823B64C4, (DWORD)XamHook, 0);
			PatchInJump((DWORD*)0x82253140, (DWORD)SV_ExecuteClientCommandHook, 0);
		}
		break;
	default:
		goto End;

	Dash:
		if (!DashLoaded)
		{
			if (!hasBeenOnDash)
			{
				Xbox::XNotify("Hayzen - Dashboard Detected");
				hasBeenOnDash = true;
			}

			DashLoaded = true;
		}

		goto End;
		break;
	}

End:
	return;
}

DWORD TitleIdMonitor()
{
	while (!UnloadPlugin)
	{
		if (XamGetCurrentTitleId() != CurrentTitle)
			GameLauncher(XamGetCurrentTitleId());
	}

	return 0;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		HANDLE hThread;
		DWORD dwThreadId;
		ExCreateThread(&hThread, 0, &dwThreadId, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TitleIdMonitor, 0, 2 | CREATE_SUSPENDED);
		XSetThreadProcessor(hThread, 4);
		SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
		ResumeThread(hThread);
		
		GameLauncher(XamGetCurrentTitleId());
	}

	if (dwReason == DLL_PROCESS_DETACH)
	{
		UnloadPlugin = true;

		Sleep(250);
	}

	return TRUE;
}