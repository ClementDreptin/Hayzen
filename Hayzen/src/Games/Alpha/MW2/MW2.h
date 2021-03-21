#pragma once

#include "Games\Alpha\MW2\Structs.h"
#include "Games\Alpha\MW2\Client.h"

namespace Alpha
{
namespace MW2
{
	// Variables
	extern bool HasGameBegun;
	extern std::unordered_map<int, Client> Clients;

	// Functions
	void Init();
	void SetupGame(int clientNum);
	void ResetGame(int clientNum);
	bool Verify(int clientNum);
	void SafeReset(); // Resets everything when the game was not reset properly

	// Hooks and their stubs
	void Scr_NotifyStub(gentity_s* entity, unsigned short stringValue, unsigned int paramCount);
	void Scr_NotifyHook(gentity_s* entity, unsigned short stringValue, unsigned int paramCount);

	void SV_ExecuteClientCommandStub(int client, const char* s, int clientOK, int fromOldServer);
	void SV_ExecuteClientCommandHook(int client, const char* s, int clientOK, int fromOldServer);
}
}