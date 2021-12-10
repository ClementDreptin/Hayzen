#include "pch.h"
#include "Games\Alpha\MW2\GameFunctions.h"


namespace AlphaMW2GameFunctions
{

LPCSTR (*SL_ConvertToString)(UINT stringValue) = (LPCSTR(*)(UINT))0x8229A730;

VOID (*Cbuf_AddText)(INT localClientNum, LPCSTR text) = (VOID(*)(INT, LPCSTR))0x8226F590;

bool (*Dvar_GetBool)(LPCSTR dvarName) = (bool(*)(LPCSTR))0x82303B00;

FLOAT (*Dvar_GetFloat)(LPCSTR dvarName) = (FLOAT(*)(LPCSTR))0x82303BD0;

LPCSTR (*Dvar_GetString)(LPCSTR dvarName) = (LPCSTR(*)(LPCSTR))0x82303CC0;

playerState_s* (*GetPlayerState)(INT clientNum) = (playerState_s*(*)(INT))0x8222C108;

bool (*Session_IsHost)(DWORD sessionDataPtr, INT clientNum) = (bool(*)(DWORD, INT))0x82388338;

VOID (*SP_script_model)(gentity_s* mSelf) = (VOID(*)(gentity_s*))0x82250A20;

gentity_s* (*G_Spawn)() = (gentity_s*(*)())0x82258508;

VOID (*G_SetModel)(gentity_s* ent, LPCSTR modelName) = (VOID(*)(gentity_s*, LPCSTR))0x82257B90;

VOID (*SV_LinkEntity)(gentity_s* gEnt) = (VOID(*)(gentity_s*))0x822C1598;

VOID (*SV_UnlinkEntity)(gentity_s* gEnt) = (VOID(*)(gentity_s*))0x822C14B0;

VOID (*SV_SetBrushModel)(gentity_s* ent) = (VOID(*)(gentity_s*))0x822B63B8;

gentity_s* (*SV_AddTestClient)() = (gentity_s*(*)())0x822B5F00;

VOID (*SV_ExecuteClientCommand)(INT client, LPCSTR s, INT clientOK, INT fromOldServer) = (VOID(*)(INT, LPCSTR, INT, INT))0x822B4700;

VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles) = (VOID(*)(gentity_s*, PFLOAT, PFLOAT))0x8222E5A0;

gclient_s* GetGClient(INT clientNum)
{
    return (gclient_s*)(0x82F01480 + sizeof(gclient_s) * clientNum);
}

gentity_s* GetEntity(INT entNum)
{
    return (gentity_s*)(0x82D47D80 + sizeof(gentity_s) * entNum);
}

bool IsHost(INT clientNum)
{
    return Session_IsHost(0x83A06F28, clientNum);
}

gentity_s* GetCurrentMapBrushModel()
{
    std::string strMapName = Dvar_GetString("ui_mapname");

    if (strMapName == "mp_afghan")
        return (gentity_s*)0x82D60880;
    if (strMapName == "mp_checkpoint")
        return (gentity_s*)0x82D76B80;
    if (strMapName == "mp_derail")
        return (gentity_s*)0x82D95800;
    if (strMapName == "mp_estate")
        return (gentity_s*)0x82D5B600;
    if (strMapName == "mp_favela")
        return (gentity_s*)0x82D9A300;
    if (strMapName == "mp_highrise")
        return (gentity_s*)0x82DC4380;
    if (strMapName == "mp_invasion")
        return (gentity_s*)0x82D7E100;
    if (strMapName == "mp_quarry")
        return (gentity_s*)0x82D70F00;
    if (strMapName == "mp_rundown")
        return (gentity_s*)0x82D93780;
    if (strMapName == "mp_rust")
        return (gentity_s*)0x82D5A980;
    if (strMapName == "mp_boneyard")
        return (gentity_s*)0x82D64E80;
    if (strMapName == "mp_nightshift")
        return (gentity_s*)0x82D58180;
    if (strMapName == "mp_subbase")
        return (gentity_s*)0x82D7CF80;
    if (strMapName == "mp_terminal")
        return (gentity_s*)0x82D61A00;
    if (strMapName == "mp_underpass")
        return (gentity_s*)0x82D52000;
    else
        return nullptr;
}

}
