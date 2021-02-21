#pragma once

namespace MW2
{
	extern void(_cdecl *Cmd_RegisterNotification)(int clientNum, const char* notify, const char* command);

	extern const char*(_cdecl *SL_ConvertToString)(unsigned int stringValue);

	extern void(_cdecl *SV)(int clientNum, int type, const char* text);

	extern bool(_cdecl *Dvar_GetBool)(const char* DvarName);
}