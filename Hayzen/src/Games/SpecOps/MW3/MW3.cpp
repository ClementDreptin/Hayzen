#include "pch.h"
#include "Games\SpecOps\MW3\MW3.h"

#include "Games\SpecOps\MW3\Events.h"

namespace SpecOps
{
namespace MW3
{
	bool HasGameBegun = false;
	std::unordered_map<int, Client> Clients;

	__declspec(naked) void DrawMenuDebugTextStub(int dc)
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

	__declspec(naked) void ClientCommandStub(int clientNum, const char* s)
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

	__declspec(naked) void PlayerCmd_AllowJumpStub()
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
		Xam::XNotify("Hayzen - MW3 Spec Ops Detected");

		Sleep(200);

		Memory::HookFunctionStart((DWORD*)0x822E0488, (DWORD*)DrawMenuDebugTextStub, (DWORD)DrawMenuDebugTextHook);
		Memory::HookFunctionStart((DWORD*)0x821FEFB0, (DWORD*)ClientCommandStub, (DWORD)ClientCommandHook);
		Memory::HookFunctionStart((DWORD*)0x821FA680, (DWORD*)PlayerCmd_AllowJumpStub, (DWORD)PlayerCmd_AllowJumpHook);
	}

	void SetupGame(int clientNum)
	{
		Verify(clientNum);
		HasGameBegun = true;
	}

	bool Verify(int clientNum)
	{
		if (clientNum < 0 || clientNum > 17)
			return false;

		if (Clients.find(clientNum) != Clients.end() && Clients[clientNum].IsInitialized())
			return false;

		Clients[clientNum] = Client(clientNum);

		return true;
	}

	void SafeReset()
	{
		if (Clients.size() != 0)
			Clients.clear();

		if (HasGameBegun)
			HasGameBegun = false;
	}

	void DrawMenuDebugTextHook(int dc)
	{
		DrawMenuDebugTextStub(dc);

		for (size_t i = 0; i < Clients.size(); i++)
			Clients[i].GetMenu().Update();
	}

	void ClientCommandHook(int clientNum, const char* s)
	{
		ClientCommandStub(clientNum, s);

		if (!strcmp(s, GAME_START))
			SetupGame(clientNum);
		
		if (HasGameBegun && Clients.find(clientNum) != Clients.end())
			Clients[clientNum].GetMenu().OnEvent(s);
	}

	void PlayerCmd_AllowJumpHook()
	{
		/**
		 * Making the PlayerCmd_AllowJump function not do anything so that you can jump in
		 * missions where you normally can't. This is a bad practice and may have side effects.
		 */
		return;
	}
}
}