#pragma once

#include "Games/SpecOps/MW3/Structs.h"

// Spec Ops MW3 function pointers to functions from the game.
namespace SpecOpsMW3
{
namespace Game
{

extern void (*SV_GameSendServerCommand)(int clientNum, const char *text);

extern void (*Cbuf_AddText)(int localClientNum, const char *text);

extern playerState_s *(*GetPlayerState)(int num);

extern void (*SetClientOrigin)(gentity_s *ent, const float *origin);

extern void (*SetClientViewAngle)(gentity_s *ent, const float *angle);

void iPrintLn(int clientNum, const std::string &text);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(int entNum);

}
}
