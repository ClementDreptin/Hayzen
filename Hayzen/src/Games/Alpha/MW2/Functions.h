#pragma once

#include "Games\Alpha\MW2\Structs.h"

namespace Alpha
{
namespace MW2
{
    extern LPCSTR (*SL_ConvertToString)(UINT stringValue);

    extern VOID (*SV)(INT clientNum, INT type, LPCSTR text);

    extern VOID (*R_AddCmdDrawText)(LPCSTR text, INT maxChars, Font_s* font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, CONST PFLOAT color, INT style);

    extern VOID (*R_AddCmdDrawStretchPic)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, CONST PFLOAT color, LPVOID material);

    extern Font_s* (*R_RegisterFont)(LPCSTR font, INT imageTrack);

    extern LPVOID (*Material_RegisterHandle)(LPCSTR name, INT imageTrack);

    extern VOID (*Cbuf_AddText)(INT localClientNum, LPCSTR text);

    extern bool (*Dvar_GetBool)(LPCSTR dvarName);

    extern FLOAT (*Dvar_GetFloat)(LPCSTR dvarName);

    extern LPCSTR (*Dvar_GetString)(LPCSTR dvarName);

    extern playerState_s* (*GetPlayerState)(INT clientNum);

    extern VOID (*SP_script_model)(gentity_s* mSelf);

    extern gentity_s* (*G_Spawn)();

    extern VOID (*G_SetModel)(gentity_s* ent, LPCSTR modelName);

    extern VOID (*SV_LinkEntity)(gentity_s* gEnt);

    extern VOID (*SV_UnlinkEntity)(gentity_s* gEnt);

    extern VOID (*SV_SetBrushModel)(gentity_s* ent);

    extern gentity_s* (*SV_AddTestClient)();

    extern VOID (*SV_ExecuteClientCommand)(INT client, LPCSTR s, INT clientOK, INT fromOldServer);

    extern VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles);

    extern gentity_s* GetEntity(INT entNum);

    extern VOID SetClientDvar(INT clientNum, CONST std::string& dvar, CONST std::string& value);

    extern VOID iPrintLn(INT clientNum, CONST std::string& text);

    extern gentity_s* GetCurrentMapBrushModel();
}
}