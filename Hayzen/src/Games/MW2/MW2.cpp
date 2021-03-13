#include "pch.h"
#include "Games\MW2\MW2.h"

#include "Games\MW2\Structs.h"
#include "Games\MW2\Functions.h"
#include "Utils\Utils.h"

bool MW2::HasGameBegun = false;
std::unordered_map<int, MW2::Client> MW2::Clients;
int MW2::HostNum = -1;

__declspec(naked) void MW2::Scr_NotifyStub(gentity_s* entity, unsigned short stringValue, unsigned int paramCount)
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

__declspec(naked) void MW2::SV_ExecuteClientCommandStub(int client, const char* s, int clientOK, int fromOldServer)
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
		Utils::Write<int>(0x83109D80 + 0x1C, 1);

		// NOP cheat protection
		Utils::Write<int>(0x8216906C, 0x60000000);
		Utils::Write<int>(0x821690E4, 0x60000000);

		Utils::HookFunctionStart((DWORD*)0x82209710, (DWORD*)Scr_NotifyStub, (DWORD)Scr_NotifyHook);
		Utils::HookFunctionStart((DWORD*)0x82253140, (DWORD*)SV_ExecuteClientCommandStub, (DWORD)SV_ExecuteClientCommandHook);
	}
}

void MW2::Reset()
{
	Clients.clear();
	HostNum = -1;
	HasGameBegun = false;
}

void MW2::SetupGame(int clientNum)
{
	if (IsHost(clientNum))
	{
		HostNum = clientNum;
		Verify(clientNum);
	}

	HasGameBegun = true;
}

bool MW2::Verify(int clientNum)
{
	if (clientNum < 0 || clientNum > 17)
		return false;

	if (Clients.find(clientNum) != Clients.end() && Clients[clientNum].IsInitialized())
		return false;

	SetClientDvar(clientNum, "loc_warnings", "0");
	SetClientDvar(clientNum, "loc_warningsUI", "0");

	Cmd_RegisterNotification(clientNum, "+actionslot 1", "dpad_up");
	Cmd_RegisterNotification(clientNum, "+actionslot 2", "dpad_down");
	Cmd_RegisterNotification(clientNum, "+actionslot 3", "dpad_left");
	Cmd_RegisterNotification(clientNum, "+actionslot 4", "dpad_right");
	Cmd_RegisterNotification(clientNum, "+gostand", "A");
	Cmd_RegisterNotification(clientNum, "+stance", "B");
	Cmd_RegisterNotification(clientNum, "+smoke", "LB");
	Cmd_RegisterNotification(clientNum, "+frag", "RB");

	Clients[clientNum] = Client(clientNum);

	return true;
}

void MW2::Scr_NotifyHook(gentity_s* entity, unsigned short stringValue, unsigned int paramCount)
{
	Scr_NotifyStub(entity, stringValue, paramCount);

	int clientNum = entity->state.number;

	const char* notify = SL_ConvertToString(stringValue);

	if (!strcmp(notify, "begin"))
		SetupGame(clientNum);

	if (HasGameBegun && clientNum >= 0 && clientNum <= 17)
		Clients[clientNum].GetMenu().OnEvent(notify);
}

void MW2::SV_ExecuteClientCommandHook(int client, const char* s, int clientOK, int fromOldServer)
{
	SV_ExecuteClientCommandStub(client, s, clientOK, fromOldServer);

	int clientNum = (client - Utils::Read<int>(0x83623B98)) / 0x97F80;

	if (!strcmp(s, "disconnect") && clientNum == HostNum)
		Reset();
}