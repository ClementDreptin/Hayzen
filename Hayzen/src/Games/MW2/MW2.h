#pragma once

#include "Games\MW2\Structs.h"
#include "Games\MW2\Client.h"

namespace MW2
{
	// Variables
	extern std::unordered_map<int, Client> Clients;

	// Functions
	void Init();
	void SetupGame(int clientNum);
	void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value);

	// Hooks and their stubs
	void Scr_NotifyStub(gentity_s* entity, unsigned short stringValue, unsigned int paramCount);
	void Scr_NotifyHook(gentity_s* entity, unsigned short stringValue, unsigned int paramCount);
}