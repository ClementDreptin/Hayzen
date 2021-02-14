#pragma once

namespace MW2
{
	void Init();

	// Hooks
	void Menu_PaintAllHook(const void* args, int unknown);
	void SV_ExecuteClientCommandHook(unsigned long client, const char * s, bool clientOK);
}