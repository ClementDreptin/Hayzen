#pragma once

#include "Games/NX1/Structs.h"

// NX1 function pointers to functions from the game.
namespace NX1
{
namespace Game
{

extern const char *(*SL_ConvertToString)(uint32_t stringValue);

extern void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text);

extern void (*Cbuf_AddText)(int localClientNum, const char *text);

extern const char *(*Dvar_GetString)(const char *dvarName);

extern float (*Dvar_GetFloat)(const char *dvarName);

extern playerState_s *(*GetPlayerState)(int clientNum);

extern bool (*Session_IsHost)(uintptr_t sessionDataPtr, int clientNum);

extern void (*SP_script_model)(gentity_s *mSelf);

extern gentity_s *(*G_Spawn)();

extern void (*G_SetModel)(gentity_s *ent, const char *modelName);

extern void (*SV_LinkEntity)(gentity_s *gEnt);

extern void (*SV_UnlinkEntity)(gentity_s *gEnt);

extern bool (*SV_SetBrushModel)(gentity_s *ent);

extern gentity_s *(*SV_AddTestClient)();

extern void (*SV_ExecuteClientCommand)(client_t *client, const char *s, int clientOK, int fromOldServer);

extern void (*SetClientOrigin)(gentity_s *ent, const float *origin);

extern void (*SetClientViewAngle)(gentity_s *ent, const float *angle);

extern bool (*UI_AnyMenuActive)(int localClientNum);

void iPrintLn(int clientNum, const std::string &text);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(uint32_t entNum);

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value);

bool IsHost(int clientNum);

gentity_s *GetCurrentMapBrushModel();

}
}
