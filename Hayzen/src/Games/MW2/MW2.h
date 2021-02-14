#pragma once

namespace MW2
{
	void Init();

	// Hooks
	DWORD XamInputGetStateHook(DWORD dwUserIndex, PXINPUT_STATE pState);
	void SV_ExecuteClientCommandHook(unsigned long client, const char * s, bool clientOK);
}