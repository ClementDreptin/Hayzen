#pragma once

#include "Games\SpecOps\MW3\Structs.h"

namespace SpecOps
{
namespace MW3
{
	extern void (*Cmd_RegisterNotification)(int clientNum, const char* notify, const char* command);

	extern const char* (*SL_ConvertToString)(unsigned int stringValue);

	extern void (*SV)(int clientNum, const char* text);

	extern bool (*Dvar_GetBool)(const char* dvarName);

	extern int (*Dvar_GetInt)(const char* dvarName);

	extern const char* (*Dvar_GetString)(const char* dvarName);

	extern void (*R_AddCmdDrawText)(const char* text, int maxChars, Font_s* font, float x, float y, float xScale, float yScale, float rotation, const float* color, int style);

	extern void (*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* color, void* material);

	extern Font_s* (*R_RegisterFont)(const char* font, int imageTrack);

	extern void* (*Material_RegisterHandle)(const char* name, int imageTrack);

	extern game_hudelem_s* (*HudElem_Alloc)(int clientNum);

	extern void (*HudElem_Free)(game_hudelem_s* hud);

	extern int (*G_MaterialIndex)(const char* name);

	extern int (*G_FindLocalizedConfigstringIndex)(const char* string);

	extern playerState_s* (*SV_GetPlayerstateForClientNum)(int num);

	extern void (*TeleportPlayer)(gentity_s* player, float* origin, float* angles);

	extern gclient_s* GetGClient(int clientNum);

	extern gentity_s* GetEntity(int entNum);

	extern void SetClientDvar(int clientNum, const std::string& dvar, const std::string& value);

	extern void iPrintLn(int clientNum, const std::string& text);
}
}