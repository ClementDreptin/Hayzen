#pragma once

#include "Games\MW3\Structs.h"

namespace MW3
{
    extern const char* (*SL_ConvertToString)(unsigned int stringValue);

    extern void (*SV)(int clientNum, int type, const char* text);

    extern const char* (*Dvar_GetString)(const char* dvarName);

    extern game_hudelem_s* (*HudElem_Alloc)(int clientNum, int teamNum);

    extern void (*HudElem_Free)(game_hudelem_s* hud);

    extern int (*G_MaterialIndex)(const char* name);

    extern int (*G_LocalizedStringIndex)(const char* string);

    extern clientState_s* (*GetClientState)(int clientNum);

    extern playerState_s* (*GetPlayerState)(int clientNum);

    extern bool (*Session_IsHost)(DWORD sessionDataPtr, int clientNum);

    extern void (*SP_script_model)(gentity_s* mSelf);

    extern gentity_s* (*G_Spawn)();

    extern void (*G_SetModel)(gentity_s* ent, const char* modelName);

    extern void (*SV_LinkEntity)(gentity_s* gEnt);

    extern void (*SV_UnlinkEntity)(gentity_s* gEnt);

    extern void (*SV_SetBrushModel)(gentity_s* ent);

    extern void (*TeleportPlayer)(gentity_s* player, float* origin, float* angles);

    extern gclient_s* GetGClient(int clientNum);

    extern gentity_s* GetEntity(int entNum);

    extern void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value);

    extern void iPrintLn(int clientNum, const std::string& text);

    extern bool IsHost(int clientNum);

    extern gentity_s* GetCurrentMapBrushModel();
}