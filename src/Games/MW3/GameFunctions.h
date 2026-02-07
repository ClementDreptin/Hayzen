#pragma once

#include "Games/MW3/Structs.h"

// MW3 TU24 function pointers to functions from the game.
namespace MW3
{
namespace Game
{

extern const char *(*SL_ConvertToString)(uint32_t stringValue);

extern void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text);

extern void (*Cbuf_AddText)(int localClientNum, const char *text);

extern bool (*Dvar_GetBool)(const char *dvarName);

extern const char *(*Dvar_GetString)(const char *dvarName);

extern playerState_s *(*GetPlayerState)(int clientNum);

extern bool (*Session_IsHost)(uintptr_t sessionDataPtr, int clientNum);

extern void (*SP_script_model)(gentity_s *mSelf);

extern gentity_s *(*G_Spawn)();

extern void (*G_SetModel)(gentity_s *ent, const char *modelName);

extern void (*SV_LinkEntity)(gentity_s *gEnt);

extern void (*SV_UnlinkEntity)(gentity_s *gEnt);

extern bool (*SV_SetBrushModel)(gentity_s *ent);

extern void (*SetClientOrigin)(gentity_s *ent, const float *origin);

extern void (*SetClientViewAngle)(gentity_s *ent, const float *angle);

extern bool (*UI_AnyMenuActive)(int localClientNum);

extern uint32_t (*GetProtocolVersion)();

extern uint32_t (*BG_NetDataChecksum)();

extern int (*G_irand)(int min, int max);

extern void (*SV_Cmd_TokenizeString)(const char *text_in);

extern void (*SV_Cmd_EndTokenizedString)();

extern void (*SV_DirectConnect)(netadr_t from);

extern void (*SV_SendClientGameState)(client_t *client);

extern void (*SV_ClientEnterWorld)(client_t *client, usercmd_s *cmd);

extern void (*SV_ExecuteClientCommand)(client_t *cl, const char *s, int clientOK, int fromOldServer);

gentity_s *SV_AddTestClient();

void iPrintLn(int clientNum, const std::string &text);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(uint32_t entNum);

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value);

bool IsHost(int clientNum);

gentity_s *GetCurrentMapCrateBrushModel();

}
}
