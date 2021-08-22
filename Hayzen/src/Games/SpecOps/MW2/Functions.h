#pragma once

#include "Games\SpecOps\MW2\Structs.h"

namespace SpecOps
{
namespace MW2
{
    extern VOID (*SV)(INT clientNum, LPCSTR text);

    extern VOID (*Cbuf_AddText)(INT localClientNum, LPCSTR text);

    extern VOID (*R_AddCmdDrawText)(LPCSTR text, INT maxChars, Font_s* font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, CONST PFLOAT color, INT style);

    extern VOID (*R_AddCmdDrawStretchPic)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, CONST PFLOAT color, LPVOID material);

    extern Font_s* (*R_RegisterFont)(LPCSTR font, INT imageTrack);

    extern LPVOID (*Material_RegisterHandle)(LPCSTR name, INT imageTrack);

    extern playerState_s* (*SV_GetPlayerstateForClientNum)(INT num);

    extern VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles);

    extern gclient_s* GetGClient(INT clientNum);

    extern gentity_s* GetEntity(INT entNum);

    extern VOID iPrintLn(INT clientNum, CONST std::string& text);
}
}