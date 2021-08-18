#pragma once

#include "Games\SpecOps\MW3\Structs.h"

namespace SpecOps
{
namespace MW3
{
    extern void (*SV)(int clientNum, const char* text);

    extern void (*R_AddCmdDrawText)(const char* text, int maxChars, Font_s* font, float x, float y, float xScale, float yScale, float rotation, const float* color, int style);

    extern void (*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float* color, void* material);

    extern Font_s* (*R_RegisterFont)(const char* font, int imageTrack);

    extern void* (*Material_RegisterHandle)(const char* name, int imageTrack);

    extern playerState_s* (*SV_GetPlayerstateForClientNum)(int num);

    extern void (*TeleportPlayer)(gentity_s* player, float* origin, float* angles);

    extern gclient_s* GetGClient(int clientNum);

    extern gentity_s* GetEntity(int entNum);

    extern void iPrintLn(int clientNum, const std::string& text);
}
}