#pragma once

#include "Games\MW2\Structs.h"

using namespace MW2Structs;


// MW2 TU9 function pointers to functions from the game.
namespace MW2GameFunctions
{

extern const char *(*SL_ConvertToString)(uint32_t stringValue);

extern void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text);

extern bool (*Dvar_GetBool)(const char *dvarName);

extern const char *(*Dvar_GetString)(const char *dvarName);

extern clientState_s *(*GetClientState)(int clientNum);

extern playerState_s *(*GetPlayerState)(int clientNum);

extern bool (*Session_IsHost)(DWORD sessionDataPtr, int clientNum);

extern void (*SP_script_model)(gentity_s *mSelf);

extern gentity_s *(*G_Spawn)();

extern void (*G_SetModel)(gentity_s *ent, const char *modelName);

extern void (*SV_LinkEntity)(gentity_s *gEnt);

extern void (*SV_UnlinkEntity)(gentity_s *gEnt);

extern void (*SV_SetBrushModel)(gentity_s *ent);

extern gentity_s *(*SV_AddTestClient)();

extern void (*SV_ExecuteClientCommand)(int client, const char *s, int clientOK, int fromOldServer);

extern void (*TeleportPlayer)(gentity_s *player, float *origin, float *angles);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(int entNum);

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value);

bool IsHost(int clientNum);

gentity_s *GetCurrentMapBrushModel();

}
