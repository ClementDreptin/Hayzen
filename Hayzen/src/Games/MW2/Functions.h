#pragma once

#include "Games\MW2\Structs.h"

namespace MW2
{
	extern void(_cdecl *Cmd_RegisterNotification)(int clientNum, const char* notify, const char* command);

	extern const char*(_cdecl *SL_ConvertToString)(unsigned int stringValue);

	extern void(_cdecl *SV)(int clientNum, int type, const char* text);

	extern bool(_cdecl *Dvar_GetBool)(const char* DvarName);

	extern game_hudelem_s*(_cdecl *HudElem_Alloc)(int clientNum, int teamNum);

	extern void(_cdecl *HudElem_Free)(game_hudelem_s* hud);

	extern int(__cdecl *G_MaterialIndex)(const char* name);

	extern int(_cdecl *G_LocalizedStringIndex)(const char* string);

	extern clientState_s*(_cdecl *GetClientState)(int clientNum);
}