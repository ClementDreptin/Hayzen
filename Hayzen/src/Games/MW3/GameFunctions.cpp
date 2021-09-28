#include "pch.h"
#include "Games\MW3\GameFunctions.h"


namespace MW3GameFunctions
{

LPCSTR (*SL_ConvertToString)(UINT stringValue) = (LPCSTR(*)(UINT))0x822B5120;

LPCSTR (*Dvar_GetString)(LPCSTR dvarName) = (LPCSTR(*)(LPCSTR))0x8232E488;

clientState_s* (*GetClientState)(INT clientNum) = (clientState_s*(*)(INT))0x82244130;

playerState_s* (*GetPlayerState)(INT clientNum) = (playerState_s*(*)(INT))0x82244148;

bool (*Session_IsHost)(DWORD sessionDataPtr, INT clientNum) = (bool(*)(DWORD, INT))0x823BFE78;

VOID (*SP_script_model)(gentity_s* mSelf) = (VOID(*)(gentity_s*))0x82268138;

gentity_s*(*G_Spawn)() = (gentity_s*(*)())0x8226F378;

VOID (*G_SetModel)(gentity_s* ent, LPCSTR modelName) = (VOID(*)(gentity_s*, LPCSTR))0x8226EAA0;

VOID (*SV_LinkEntity)(gentity_s* gEnt) = (VOID(*)(gentity_s*))0x822D6E78;

VOID (*SV_UnlinkEntity)(gentity_s* gEnt) = (VOID(*)(gentity_s*))0x822D6D90;

VOID (*SV_SetBrushModel)(gentity_s* ent) = (VOID(*)(gentity_s*))0x822C95C8;

VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles) = (VOID(*)(gentity_s*, PFLOAT, PFLOAT))0x82246048;

gclient_s* GetGClient(INT clientNum)
{
    return (gclient_s*)(0x82F99580 + sizeof(gclient_s) * clientNum);
}

gentity_s* GetEntity(INT entNum)
{
    return (gentity_s*)(0x82DCCC80 + sizeof(gentity_s) * entNum);
}

bool IsHost(INT clientNum)
{
    return Session_IsHost(0x83BC0148, clientNum);
}

gentity_s* GetCurrentMapBrushModel()
{
    std::string strMapName = Dvar_GetString("ui_mapname");

    if (strMapName == "mp_seatown")
        return (gentity_s*)0x82DD1280;
    if (strMapName == "mp_mogadishu")
        return (gentity_s*)0x82E08A00;
    if (strMapName == "mp_exchange")
        return (gentity_s*)0x82E14580;
    if (strMapName == "mp_radar")
        return (gentity_s*)0x82DD3A80;
    if (strMapName == "mp_terminal_cls")
        return (gentity_s*)0x82DF9C80;
    else
        return (gentity_s*)0x82DD1500;
}

}
