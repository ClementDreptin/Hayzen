#pragma once

#include "Games\MW3\Structs.h"

using namespace MW3Structs;


// MW3 TU24 function pointers to functions from the game.
namespace MW3GameFunctions
{

extern LPCSTR (*SL_ConvertToString)(UINT stringValue);

extern LPCSTR (*Dvar_GetString)(LPCSTR dvarName);

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

gclient_s* GetGClient(INT clientNum);

gentity_s* GetEntity(INT entNum);

bool IsHost(INT clientNum);

gentity_s* GetCurrentMapBrushModel();

}
