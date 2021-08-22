#include "pch.h"
#include "Games\SpecOps\MW3\Functions.h"

namespace SpecOps
{
namespace MW3
{
    VOID (*SV)(INT clientNum, LPCSTR text) = (VOID(*)(INT, LPCSTR))0x82371680;

    VOID (*R_AddCmdDrawText)(LPCSTR text, INT maxChars, Font_s* font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, CONST PFLOAT color, INT style) =
        (VOID(*)(LPCSTR , INT, Font_s* , FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, CONST PFLOAT, INT))0x823F4E30;

    VOID (*R_AddCmdDrawStretchPic)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, CONST PFLOAT color, LPVOID material) =
        (VOID(*)(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, CONST PFLOAT, LPVOID))0x823F4878;

    Font_s* (*R_RegisterFont)(LPCSTR font, INT imageTrack) = (Font_s*(*)(LPCSTR, INT))0x823DD130;

    LPVOID (*Material_RegisterHandle)(LPCSTR name, INT imageTrack) = (LPVOID(*)(LPCSTR, INT))0x823E95E8;

    playerState_s* (*SV_GetPlayerstateForClientNum)(INT num) = (playerState_s*(*)(INT))0x82371650;

    VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles) = (VOID(*)(gentity_s*, PFLOAT, PFLOAT))0x8220C960;

    gclient_s* GetGClient(INT clientNum)
    {
        return (gclient_s*)(0x83150090 + sizeof(gclient_s) * clientNum);
    }

    gentity_s* GetEntity(INT entNum)
    {
        return (gentity_s*)(0x82F66560 + sizeof(gentity_s) * entNum);
    }

    VOID iPrintLn(INT clientNum, CONST std::string& text)
    {
        SV(clientNum, Formatter::Format("gm \"%s\"", text.c_str()).c_str());
    }
}
}