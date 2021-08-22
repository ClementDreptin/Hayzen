#pragma once

#include "Games\MW3\Structs.h"

namespace MW3
{
    extern LPCSTR (*SL_ConvertToString)(UINT stringValue);

    extern VOID (*SV)(INT clientNum, INT type, LPCSTR text);

    extern LPCSTR (*Dvar_GetString)(LPCSTR dvarName);

    extern game_hudelem_s* (*HudElem_Alloc)(INT clientNum, INT teamNum);

    extern VOID (*HudElem_Free)(game_hudelem_s* hud);

    extern INT (*G_MaterialIndex)(LPCSTR name);

    extern INT (*G_LocalizedStringIndex)(LPCSTR string);

    extern clientState_s* (*GetClientState)(INT clientNum);

    extern playerState_s* (*GetPlayerState)(INT clientNum);

    extern bool (*Session_IsHost)(DWORD sessionDataPtr, INT clientNum);

    extern VOID (*SP_script_model)(gentity_s* mSelf);

    extern gentity_s* (*G_Spawn)();

    extern VOID (*G_SetModel)(gentity_s* ent, LPCSTR modelName);

    extern VOID (*SV_LinkEntity)(gentity_s* gEnt);

    extern VOID (*SV_UnlinkEntity)(gentity_s* gEnt);

    extern VOID (*SV_SetBrushModel)(gentity_s* ent);

    extern VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles);

    extern gclient_s* GetGClient(INT clientNum);

    extern gentity_s* GetEntity(INT entNum);

    extern VOID SetClientDvar(INT clientNum, CONST std::string& dvar, CONST std::string& value);

    extern VOID iPrintLn(INT clientNum, CONST std::string& text);

    extern bool IsHost(INT clientNum);

    extern gentity_s* GetCurrentMapBrushModel();
}