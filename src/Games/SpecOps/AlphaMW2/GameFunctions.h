#pragma once

#include "Games/Common/Structs.h"
#include "Games/SpecOps/AlphaMW2/Structs.h"

// Spec Ops MW2 Alpha function pointers to functions from the game.
namespace SpecOpsAlphaMW2
{
namespace Game
{

extern void (*SV_GameSendServerCommand)(int clientNum, const char *text);

extern void (*Cbuf_AddText)(int localClientNum, const char *text);

extern float (*Dvar_GetFloat)(const char *dvarName);

extern void (*Dvar_ForEach)(void (*callback)(const dvar_t *dvar, void *data), void *userData);

extern playerState_s *(*GetPlayerState)(int num);

extern void (*SetClientOrigin)(gentity_s *ent, const float *origin);

extern void (*SetClientViewAngle)(gentity_s *ent, const float *angle);

extern bool (*UI_AnyMenuActive)(int localClientNum);

void iPrintLn(int clientNum, const std::string &text);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(uint32_t entNum);

}
}
