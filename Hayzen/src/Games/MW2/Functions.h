#pragma once

#include "Games\MW2\Structs.h"

namespace MW2
{
	extern const char* (*SL_ConvertToString)(unsigned int stringValue);

	extern void (*SV)(int clientNum, int type, const char* text);

	extern void (*R_AddCmdDrawText)(const char* text, int maxChars, Font_s* font, float x, float y, float xScale, float yScale, float rotation, const float* color, int style);

	extern void (*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* color, void* material);

	extern Font_s* (*R_RegisterFont)(const char* font, int imageTrack);

	extern void* (*Material_RegisterHandle)(const char* name, int imageTrack);

	extern bool (*Dvar_GetBool)(const char* dvarName);

	extern const char* (*Dvar_GetString)(const char* dvarName);

	extern clientState_s* (*GetClientState)(int clientNum);

	extern playerState_s* (*GetPlayerState)(int clientNum);

	extern bool (*Session_IsHost)(DWORD sessionDataPtr, int clientNum);

	extern void (*SP_script_model)(gentity_s* mSelf);

	extern gentity_s* (*G_Spawn)();

	extern void (*G_SetModel)(gentity_s* ent, const char* modelName);

	extern void (*SV_LinkEntity)(gentity_s* gEnt);

	extern void (*SV_UnlinkEntity)(gentity_s* gEnt);

	extern void (*SV_SetBrushModel)(gentity_s* ent);

	extern gentity_s* (*SV_AddTestClient)();

	extern void (*SV_ExecuteClientCommand)(int client, const char* s, int clientOK, int fromOldServer);

	extern void (*TeleportPlayer)(gentity_s* player, float* origin, float* angles);

	extern gclient_s* GetGClient(int clientNum);

	extern gentity_s* GetEntity(int entNum);

	extern void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value);

	extern void iPrintLn(int clientNum, const std::string& text);

	extern bool IsHost(int clientNum);

	extern gentity_s* GetCurrentMapBrushModel();
}