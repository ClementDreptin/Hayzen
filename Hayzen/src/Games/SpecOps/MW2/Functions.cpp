#include "pch.h"
#include "Games\SpecOps\MW2\Functions.h"

namespace SpecOps
{
namespace MW2
{
    VOID (*SV)(INT clientNum, LPCSTR text) = (VOID(*)(INT, LPCSTR))0x8233CB60;

    VOID (*Cbuf_AddText)(INT localClientNum, LPCSTR text) = (VOID(*)(INT, LPCSTR))0x8227CF90;

    VOID (*R_AddCmdDrawText)(LPCSTR text, INT maxChars, Font_s* font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, CONST PFLOAT color, INT style) =
        (VOID(*)(LPCSTR , INT, Font_s* , FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, CONST PFLOAT, INT))0x82391F00;

    VOID (*R_AddCmdDrawStretchPic)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, CONST PFLOAT color, LPVOID material) =
        (VOID(*)(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, CONST PFLOAT, LPVOID))0x82391620;

    Font_s* (*R_RegisterFont)(LPCSTR font, INT imageTrack) = (Font_s*(*)(LPCSTR, INT))0x8238B5A8;

    LPVOID (*Material_RegisterHandle)(LPCSTR name, INT imageTrack) = (LPVOID(*)(LPCSTR, INT))0x8238BE08;

    playerState_s* (*SV_GetPlayerstateForClientNum)(INT num) = (playerState_s*(*)(INT))0x8233CB30;

    VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles) = (VOID(*)(gentity_s*, PFLOAT, PFLOAT))0x821FF588;

    gclient_s* GetGClient(INT clientNum)
    {
        return (gclient_s*)(0x82EAC5E8 + sizeof(gclient_s) * clientNum);
    }

    gentity_s* GetEntity(INT entNum)
    {
        return (gentity_s*)(0x82CC67B8 + sizeof(gentity_s) * entNum);
    }

    VOID iPrintLn(INT clientNum, CONST std::string& text)
    {
        SV(clientNum, Formatter::Format("gm \"%s\"", text.c_str()).c_str());
    }
}
}