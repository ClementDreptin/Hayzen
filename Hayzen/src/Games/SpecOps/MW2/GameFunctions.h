#pragma once

#include "Games\SpecOps\MW2\Structs.h"

using namespace SpecOpsMW2Structs;


// Spec Ops MW2 function pointers to functions from the game.
namespace SpecOpsMW2GameFunctions
{

extern void (*SV_GameSendServerCommand)(int clientNum, const char *text);

extern void (*Cbuf_AddText)(int localClientNum, const char *text);

extern playerState_s *(*SV_GetPlayerstateForClientNum)(int num);

extern void (*TeleportPlayer)(gentity_s *player, const float *origin, const float *angles);

void iPrintLn(int clientNum, const std::string &text);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(int entNum);

}
