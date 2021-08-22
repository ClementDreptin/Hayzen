#include "pch.h"
#include "Games\Alpha\MW2\Functions.h"

#include "Games\Alpha\MW2\Structs.h"

namespace Alpha
{
namespace MW2
{
    LPCSTR (*SL_ConvertToString)(UINT stringValue) = (LPCSTR(*)(UINT))0x8229A730;

    VOID (*SV)(INT clientNum, INT type, LPCSTR text) = (VOID(*)(INT, INT, LPCSTR))0x822B6140;

    PFLOAT test = 0;

    VOID (*R_AddCmdDrawText)(LPCSTR text, INT maxChars, Font_s* font, FLOAT x, FLOAT y, FLOAT xScale, FLOAT yScale, FLOAT rotation, CONST PFLOAT color, INT style) =
        (VOID(*)(LPCSTR , INT, Font_s* , FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, CONST PFLOAT, INT))0x823BB4D8;

    VOID (*R_AddCmdDrawStretchPic)(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT s0, FLOAT t0, FLOAT s1, FLOAT t1, CONST PFLOAT color, LPVOID material) =
        (VOID(*)(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, CONST PFLOAT, LPVOID))0x823BAC18;

    Font_s* (*R_RegisterFont)(LPCSTR font, INT imageTrack) = (Font_s*(*)(LPCSTR, INT))0x823B6D58;

    LPVOID (*Material_RegisterHandle)(LPCSTR name, INT imageTrack) = (LPVOID(*)(LPCSTR, INT))0x823B6928;

    VOID (*Cbuf_AddText)(INT localClientNum, LPCSTR text) = (VOID(*)(INT, LPCSTR))0x8226F590;

    bool (*Dvar_GetBool)(LPCSTR dvarName) = (bool(*)(LPCSTR))0x82303B00;

    FLOAT (*Dvar_GetFloat)(LPCSTR dvarName) = (FLOAT(*)(LPCSTR))0x82303BD0;

    LPCSTR (*Dvar_GetString)(LPCSTR dvarName) = (LPCSTR(*)(LPCSTR))0x82303CC0;

    playerState_s* (*GetPlayerState)(INT clientNum) = (playerState_s*(*)(INT))0x8222C108;

    VOID (*SP_script_model)(gentity_s* mSelf) = (VOID(*)(gentity_s*))0x82250A20;

    gentity_s* (*G_Spawn)() = (gentity_s*(*)())0x82258508;

    VOID (*G_SetModel)(gentity_s* ent, LPCSTR modelName) = (VOID(*)(gentity_s*, LPCSTR))0x82257B90;

    VOID (*SV_LinkEntity)(gentity_s* gEnt) = (VOID(*)(gentity_s*))0x822C1598;

    VOID (*SV_UnlinkEntity)(gentity_s* gEnt) = (VOID(*)(gentity_s*))0x822C14B0;

    VOID (*SV_SetBrushModel)(gentity_s* ent) = (VOID(*)(gentity_s*))0x822B63B8;

    gentity_s* (*SV_AddTestClient)() = (gentity_s*(*)())0x822B5F00;

    VOID (*SV_ExecuteClientCommand)(INT client, LPCSTR s, INT clientOK, INT fromOldServer) = (VOID(*)(INT, LPCSTR, INT, INT))0x822B4700;

    VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles) = (VOID(*)(gentity_s*, PFLOAT, PFLOAT))0x8222E5A0;

    gentity_s* GetEntity(INT entNum)
    {
        return (gentity_s*)(0x82D47D80 + sizeof(gentity_s) * entNum);
    }

    VOID SetClientDvar(INT clientNum, CONST std::string& dvar, CONST std::string& value)
    {
        SV(clientNum, 0, Formatter::Format("v %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
    }

    VOID iPrintLn(INT clientNum, CONST std::string& text)
    {
        SV(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
    }

    gentity_s* GetCurrentMapBrushModel()
    {
        std::string mapName = Dvar_GetString("ui_mapname");

        if (mapName == "mp_afghan")
            return (gentity_s*)0x82D60880;
        if (mapName == "mp_checkpoint")
            return (gentity_s*)0x82D76B80;
        if (mapName == "mp_derail")
            return (gentity_s*)0x82D95800;
        if (mapName == "mp_estate")
            return (gentity_s*)0x82D5B600;
        if (mapName == "mp_favela")
            return (gentity_s*)0x82D9A300;
        if (mapName == "mp_highrise")
            return (gentity_s*)0x82DC4380;
        if (mapName == "mp_invasion")
            return (gentity_s*)0x82D7E100;
        if (mapName == "mp_quarry")
            return (gentity_s*)0x82D70F00;
        if (mapName == "mp_rundown")
            return (gentity_s*)0x82D93780;
        if (mapName == "mp_rust")
            return (gentity_s*)0x82D5A980;
        if (mapName == "mp_boneyard")
            return (gentity_s*)0x82D64E80;
        if (mapName == "mp_nightshift")
            return (gentity_s*)0x82D58180;
        if (mapName == "mp_subbase")
            return (gentity_s*)0x82D7CF80;
        if (mapName == "mp_terminal")
            return (gentity_s*)0x82D61A00;
        if (mapName == "mp_underpass")
            return (gentity_s*)0x82D52000;
        else
            return nullptr;
    }
}
}