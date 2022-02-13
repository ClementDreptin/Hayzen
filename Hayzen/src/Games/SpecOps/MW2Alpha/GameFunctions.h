#pragma once

#include "Games\SpecOps\MW2Alpha\Structs.h"

using namespace SpecOpsAlphaMW2Structs;


// Spec Ops MW2 Alpha function pointers to functions from the game.
namespace SpecOpsAlphaMW2GameFunctions
{

extern void (*SV_GameSendServerCommand)(int clientNum, const char *text);

extern void (*Cbuf_AddText)(int localClientNum, const char *text);

extern playerState_s *(*SV_GetPlayerstateForClientNum)(int num);

extern void (*SetClientOrigin)(gentity_s *ent, const float *origin);

extern void (*SetClientViewAngle)(gentity_s *ent, const float *angle);

void iPrintLn(int clientNum, const std::string &text);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(int entNum);

}
