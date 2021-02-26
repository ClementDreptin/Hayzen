#pragma once

#include "Games\MW2\Structs.h"

namespace MW2
{
	// Functions
	void Init();
	void SetupGame(int clientNum);
	void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value);

	// Hooks and their stubs
	void Menu_PaintAllStub(const void* args, int unknown);
	void Menu_PaintAllHook(const void* args, int unknown);

	void Scr_NotifyStub(gentity_s* entity, unsigned short stringValue, unsigned int paramCount);
	void Scr_NotifyHook(gentity_s* entity, unsigned short stringValue, unsigned int paramCount);
}