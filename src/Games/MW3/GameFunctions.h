#pragma once

#include "Games\MW3\Structs.h"

// MW3 TU24 function pointers to functions from the game.
namespace MW3
{
namespace Game
{

extern const char *(*SL_ConvertToString)(uint32_t stringValue);

extern void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text);

extern void (*Cbuf_AddText)(int localClientNum, const char *text);

extern const char *(*Dvar_GetString)(const char *dvarName);

extern clientState_s *(*GetClientState)(int clientNum);

extern playerState_s *(*GetPlayerState)(int clientNum);

extern bool (*Session_IsHost)(uintptr_t sessionDataPtr, int clientNum);

extern void (*SP_script_model)(gentity_s *mSelf);

extern gentity_s *(*G_Spawn)();

extern void (*G_SetModel)(gentity_s *ent, const char *modelName);

extern void (*SV_LinkEntity)(gentity_s *gEnt);

extern void (*SV_UnlinkEntity)(gentity_s *gEnt);

extern void (*SV_SetBrushModel)(gentity_s *ent);

extern void (*SetClientOrigin)(gentity_s *ent, const float *origin);

extern void (*SetClientViewAngle)(gentity_s *ent, const float *angle);

void iPrintLn(int clientNum, const std::string &text);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(int entNum);

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value);

bool IsHost(int clientNum);

gentity_s *GetCurrentMapBrushModel();

}
}
