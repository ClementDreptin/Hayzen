#pragma once

#include "Games\SpecOps\MW3\Structs.h"

using namespace SpecOpsMW3Structs;


// Spec Ops MW3 function pointers to functions from the game.
namespace SpecOpsMW3GameFunctions
{

extern playerState_s *(*SV_GetPlayerstateForClientNum)(int num);

extern void (*TeleportPlayer)(gentity_s *player, const float *origin, const float *angles);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(int entNum);

}
