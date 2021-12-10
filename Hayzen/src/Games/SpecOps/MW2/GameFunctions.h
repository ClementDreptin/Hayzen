#pragma once

#include "Games\SpecOps\MW2\Structs.h"

using namespace SpecOpsMW2Structs;


// Spec Ops MW2 function pointers to functions from the game.
namespace SpecOpsMW2GameFunctions
{

extern playerState_s *(*SV_GetPlayerstateForClientNum)(INT num);

extern VOID (*TeleportPlayer)(gentity_s *player, PFLOAT origin, PFLOAT angles);

gclient_s *GetGClient(INT clientNum);

gentity_s *GetEntity(INT entNum);

}
