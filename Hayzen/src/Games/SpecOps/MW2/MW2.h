#pragma once

#include "Games\SpecOps\MW2\Client.h"

namespace SpecOps
{
namespace MW2
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
	void SCR_DrawScreenFieldStub(int refreshedUI);
	void SCR_DrawScreenFieldHook(int refreshedUI);

	void ClientCommandStub(int clientNum, const char* s);
	void ClientCommandHook(int clientNum, const char* s);
}
}