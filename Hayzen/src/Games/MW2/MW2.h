#pragma once

#include "Games\MW2\Structs.h"

namespace MW2
{
	// Functions
	void Init();
	void SetupGame(int clientNum);
	void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value);

	// Variables
	extern bool GameInitialized;

	// Hooks
	void SV_ExecuteClientCommandHook(int client, const char* s, int clientOK, int fromOldServer);
	void Menu_PaintAllHook(const void* args, int unknown);
	void Scr_NotifyHook(gentity_s* entity, unsigned short stringValue, unsigned int paramCount);
}