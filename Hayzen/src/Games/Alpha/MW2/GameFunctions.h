#pragma once

#include "Games\Alpha\MW2\Structs.h"

using namespace AlphaMW2Structs;


//--------------------------------------------------------------------------------------
// MW2 Alpha function pointers to functions from the game
//--------------------------------------------------------------------------------------
namespace AlphaMW2GameFunctions
{

extern LPCSTR (*SL_ConvertToString)(UINT stringValue);

extern VOID (*Cbuf_AddText)(INT localClientNum, LPCSTR text);

extern bool (*Dvar_GetBool)(LPCSTR dvarName);

extern FLOAT (*Dvar_GetFloat)(LPCSTR dvarName);

extern LPCSTR (*Dvar_GetString)(LPCSTR dvarName);

extern playerState_s* (*GetPlayerState)(INT clientNum);

extern bool (*Session_IsHost)(DWORD sessionDataPtr, INT clientNum);

extern VOID (*SP_script_model)(gentity_s* mSelf);

extern gentity_s* (*G_Spawn)();

extern VOID (*G_SetModel)(gentity_s* ent, LPCSTR modelName);

extern VOID (*SV_LinkEntity)(gentity_s* gEnt);

extern VOID (*SV_UnlinkEntity)(gentity_s* gEnt);

extern VOID (*SV_SetBrushModel)(gentity_s* ent);

extern gentity_s* (*SV_AddTestClient)();

extern VOID (*SV_ExecuteClientCommand)(INT client, LPCSTR s, INT clientOK, INT fromOldServer);

extern VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles);

gentity_s* GetEntity(INT entNum);

bool IsHost(INT clientNum);

gentity_s* GetCurrentMapBrushModel();

}
