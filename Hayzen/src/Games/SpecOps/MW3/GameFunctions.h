#pragma once

#include "Games\SpecOps\MW3\Structs.h"

using namespace SpecOpsMW3Structs;


//--------------------------------------------------------------------------------------
// Spec Ops MW3 function pointers to functions from the game
//--------------------------------------------------------------------------------------
namespace SpecOpsMW3GameFunctions
{

extern VOID (*SV)(INT clientNum, LPCSTR text);

extern playerState_s* (*SV_GetPlayerstateForClientNum)(INT num);

extern VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles);

gclient_s* GetGClient(INT clientNum);

gentity_s* GetEntity(INT entNum);

}
