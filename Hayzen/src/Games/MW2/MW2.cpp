#include "pch.h"
#include "Games\MW2\MW2.h"

#include "Games\MW2\Structs.h"
#include "Games\MW2\Functions.h"
#include "Utils\Utils.h"
#include "Utils\Formater.h"

bool MW2::GameInitialized = false;

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

__declspec(naked) void Scr_NotifyStub(MW2::gentity_s* entity, unsigned short stringValue, unsigned int paramCount)
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
		li r3, 2
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

		//Utils::HookFunctionStart((DWORD*)0x82285C98, (DWORD*)RenderSaveStub, (DWORD)Menu_PaintAllHook);
		Utils::HookFunctionStart((DWORD*)0x82253140, (DWORD*)SV_ExecuteClientCommandStub, (DWORD)SV_ExecuteClientCommandHook);
		Utils::HookFunctionStart((DWORD*)0x82209710, (DWORD*)Scr_NotifyStub, (DWORD)Scr_NotifyHook);
	}
}

void MW2::SetupGame(int clientNum)
{
	// TODO: figure out why this prevents the game from starting
	//SetClientDvar(clientNum, "loc_warnings", "0");
	//SetClientDvar(clientNum, "loc_warningsUI", "0");

	Cmd_RegisterNotification(clientNum, "+actionslot 1", "dpad_up");
	Cmd_RegisterNotification(clientNum, "+actionslot 2", "dpad_down");
	Cmd_RegisterNotification(clientNum, "+actionslot 3", "dpad_left");
	Cmd_RegisterNotification(clientNum, "+actionslot 4", "dpad_right");
	Cmd_RegisterNotification(clientNum, "+gostand", "A");
	Cmd_RegisterNotification(clientNum, "+stance", "B");

	GameInitialized = true;
}

void MW2::SetClientDvar(int clientNum, const std::string& dvar, const std::string& value)
{
	SV(clientNum, 0, Formater::LinkChar("s %s \"%s\"", dvar, value));
}

void MW2::SV_ExecuteClientCommandHook(int client, const char* s, int clientOK, int fromOldServer)
{
	SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

	if (!strcmp(s, "disconnect"))
		GameInitialized = false;
}

void MW2::Menu_PaintAllHook(const void* args, int unknown)
{
	RenderSaveStub(args, unknown);
}

void MW2::Scr_NotifyHook(gentity_s* entity, unsigned short stringValue, unsigned int paramCount)
{
	Scr_NotifyStub(entity, stringValue, paramCount);

	int clientNum = entity->state.number;

	const char* notify = SL_ConvertToString(stringValue);

	if (!strcmp(notify, "begin"))
		SetupGame(clientNum);

	if (!strcmp(notify, "dpad_up"))
		SV(clientNum, 0, "f \"^2Dpad Up Pressed!\"");
	
	if (!strcmp(notify, "dpad_down"))
		SV(clientNum, 0, "f \"^2Dpad Down Pressed!\"");
	
	if (!strcmp(notify, "dpad_left"))
		SV(clientNum, 0, "f \"^2Dpad Left Pressed!\"");
	
	if (!strcmp(notify, "dpad_right"))
		SV(clientNum, 0, "f \"^2Dpad Right Pressed!\"");
	
	if (!strcmp(notify, "A"))
		SV(clientNum, 0, "f \"^2A Pressed!\"");
	
	if (!strcmp(notify, "B"))
		SV(clientNum, 0, "f \"^2B Pressed!\"");
}