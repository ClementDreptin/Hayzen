#pragma once

#include "Core/UI.h"
#include "Games/CoD4/Structs.h"

// CoD4 TU4 function pointers to functions from the game.
namespace CoD4
{
namespace Game
{

extern void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text);

extern void (*Cbuf_AddText)(int localClientNum, const char *text);

extern bool (*Dvar_GetBool)(const char *dvarName);

extern const char *(*Dvar_GetString)(const char *dvarName);

extern float (*Dvar_GetFloat)(const char *dvarName);

extern void (*SP_script_model)(gentity_s *mSelf);

extern gentity_s *(*G_Spawn)();

extern void (*G_SetModel)(gentity_s *ent, const char *modelName);

extern void (*SV_LinkEntity)(gentity_s *gEnt);

extern void (*SV_UnlinkEntity)(gentity_s *gEnt);

extern bool (*SV_SetBrushModel)(gentity_s *ent);

extern gentity_s *(*SV_AddTestClient)();

extern void (*SV_ExecuteClientCommand)(client_t *client, const char *s, int clientOK);

extern void (*SetClientViewAngle)(gentity_s *ent, const float *angle);

extern bool (*UI_AnyMenuActive)(int localClientNum);

extern void *(*DB_FindXAssetHeader)(int type, const char *name);

extern void (*BG_PlayerStateToEntityState)(playerState_s *ps, entityState_s *s, int snap, char handler);

int R_TextHeight(UI::Font_s *pFont);

UI::Font_s *R_RegisterFont(const char *font, int imageTrack);

HANDLE Material_RegisterHandle(const char *name, int imageTrack);

const char *SL_ConvertToString(uint32_t stringValue);

void iPrintLn(int clientNum, const std::string &text);

gclient_s *GetGClient(int clientNum);

playerState_s *GetPlayerState(int clientNum);

gentity_s *GetEntity(uint32_t entNum);

void SetClientOrigin(gentity_s *ent, const float *origin);

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value);

bool IsHost(int clientNum);

gentity_s *GetCurrentMapCrateBrushModel();

}
}
