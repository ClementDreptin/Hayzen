#pragma once

#include "Games/AlphaMW2/Structs.h"
#include "Games/Common/Structs.h"

// MW2 Alpha function pointers to functions from the game.
namespace AlphaMW2
{
namespace Game
{

extern const char *(*SL_ConvertToString)(uint32_t stringValue);

extern void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text);

extern void (*Cbuf_AddText)(int localClientNum, const char *text);

extern const char *(*Dvar_GetString)(const char *dvarName);

extern float (*Dvar_GetFloat)(const char *dvarName);

extern void (*Dvar_ForEach)(void (*callback)(const dvar_t *dvar, void *data), void *userData);

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

extern uint32_t (*Scr_LoadScript)(const char *filename);

extern int (*Scr_GetFunctionHandle)(const char *filename, const char *name);

extern uint16_t (*Scr_ExecThread)(int handle, uint32_t paramcount);

extern void (*Scr_FreeThread)(uint16_t handle);

void iPrintLn(int clientNum, const std::string &text);

gclient_s *GetGClient(int clientNum);

gentity_s *GetEntity(uint32_t entNum);

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value);

bool IsHost(int clientNum);

gentity_s *GetCurrentMapBrushModel();

}
}
