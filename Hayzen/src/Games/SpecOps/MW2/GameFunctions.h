#pragma once

#include "Games\SpecOps\MW2\Structs.h"

using namespace SpecOpsMW2Structs;


// Spec Ops MW2 function pointers to functions from the game.
namespace SpecOpsMW2GameFunctions
{

extern playerState_s *(*SV_GetPlayerstateForClientNum)(int num);

extern void (*TeleportPlayer)(gentity_s *player, float *origin, float *angles);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(int entNum);

}
