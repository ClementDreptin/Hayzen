#pragma once

#include "Games\MW2\Structs.h"

namespace MW2
{
	extern void(__cdecl *Cmd_RegisterNotification)(int clientNum, const char* notify, const char* command);

	extern const char*(__cdecl *SL_ConvertToString)(unsigned int stringValue);

	extern void(__cdecl *SV)(int clientNum, int type, const char* text);

	extern bool(__cdecl *Dvar_GetBool)(const char* dvarName);

	extern int(__cdecl *Dvar_GetInt)(const char* dvarName);

	extern const char* (_cdecl *Dvar_GetString)(const char* dvarName);

	extern game_hudelem_s*(__cdecl *HudElem_Alloc)(int clientNum, int teamNum);

	extern void(__cdecl *HudElem_Free)(game_hudelem_s* hud);

	extern int(__cdecl *G_MaterialIndex)(const char* name);

	extern int(__cdecl *G_LocalizedStringIndex)(const char* string);

	extern clientState_s*(__cdecl *GetClientState)(int clientNum);

	extern playerState_s*(__cdecl *GetPlayerState)(int clientNum);

	extern bool(__cdecl *Session_IsHost)(DWORD sessionDataPtr, int clientNum);

	extern void(__cdecl *SP_script_model)(gentity_s* mSelf);

	extern gentity_s*(__cdecl *G_Spawn)();

	extern void(__cdecl *G_SetModel)(gentity_s* ent, const char* modelName);

	extern void(__cdecl *SV_LinkEntity)(gentity_s* gEnt);

	extern void(__cdecl *SV_UnlinkEntity)(gentity_s* gEnt);

	extern void(__cdecl *SV_SetBrushModel)(gentity_s* ent);

	extern gclient_s* GetGClient(int clientNum);

	extern gentity_s* GetPlayerEntity(int clientNum);

	extern void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value);

	extern void iPrintLn(int clientNum, const std::string& text);

	extern bool IsHost(int clientNum);

	extern gentity_s* GetCurrentMapEntity();
}