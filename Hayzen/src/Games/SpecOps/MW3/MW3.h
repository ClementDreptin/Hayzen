#pragma once

#include "Games\SpecOps\MW3\Client.h"

namespace SpecOps
{
namespace MW3
{
	// Variables
	extern bool HasGameBegun;
	extern std::unordered_map<int, Client> Clients;

	// Functions
	void Init();
	void SetupGame(int clientNum);
	bool Verify(int clientNum);
	void SafeReset(); // Resets everything when the game was not reset properly

	// Hooks and their stubs
	void DrawMenuDebugTextStub(int dc);
	void DrawMenuDebugTextHook(int dc);

	void ClientCommandStub(int clientNum, const char* s);
	void ClientCommandHook(int clientNum, const char* s);

	void PlayerCmd_AllowJumpStub();
	void PlayerCmd_AllowJumpHook();
}
}