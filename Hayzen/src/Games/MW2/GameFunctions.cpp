#include "pch.h"
#include "Games\MW2\GameFunctions.h"


namespace MW2GameFunctions
{

LPCSTR (*SL_ConvertToString)(UINT stringValue) = (LPCSTR(*)(UINT))0x82241898;

VOID (*SV_GameSendServerCommand)(INT clientNum, INT type, LPCSTR text) = (VOID(*)(INT, INT, LPCSTR))0x822548D8;

bool (*Dvar_GetBool)(LPCSTR dvarName) = (bool(*)(LPCSTR))0x8229EEE8;

LPCSTR (*Dvar_GetString)(LPCSTR dvarName) = (LPCSTR(*)(LPCSTR))0x8229F0A8;

clientState_s *(*GetClientState)(INT clientNum) = (clientState_s*(*)(INT))0x821E6610;

playerState_s *(*GetPlayerState)(INT clientNum) = (playerState_s*(*)(INT))0x821E6628;

bool (*Session_IsHost)(DWORD sessionDataPtr, INT clientNum) = (bool(*)(DWORD, INT))0x82320138;

VOID (*SP_script_model)(gentity_s *mSelf) = (VOID(*)(gentity_s*))0x82206D88;

gentity_s*(*G_Spawn)() = (gentity_s*(*)())0x8220DB50;

VOID (*G_SetModel)(gentity_s *ent, LPCSTR modelName) = (VOID(*)(gentity_s*, LPCSTR))0x8220D278;

VOID (*SV_LinkEntity)(gentity_s *gEnt) = (VOID(*)(gentity_s*))0x8225F518;

VOID (*SV_UnlinkEntity)(gentity_s *gEnt) = (VOID(*)(gentity_s*))0x8225F430;

VOID (*SV_SetBrushModel)(gentity_s *ent) = (VOID(*)(gentity_s*))0x82254B50;

gentity_s *(*SV_AddTestClient)() = (gentity_s*(*)())0x82254690;

VOID (*SV_ExecuteClientCommand)(INT client, LPCSTR s, INT clientOK, INT fromOldServer) = (VOID(*)(INT, LPCSTR, INT, INT))0x82253140;

VOID (*TeleportPlayer)(gentity_s *player, PFLOAT origin, PFLOAT angles) = (VOID(*)(gentity_s*, PFLOAT, PFLOAT))0x821E8198;

gclient_s *GetGClient(INT clientNum)
{
    return (gclient_s*)(0x830CBF80 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(INT entNum)
{
    return (gentity_s*)(0x82F03600 + sizeof(gentity_s) * entNum);
}

VOID SetClientDvar(INT clientNum, CONST std::string &dvar, CONST std::string &value)
{
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("s %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(INT clientNum)
{
    return Session_IsHost(0x83AC3DB0, clientNum);
}

gentity_s *GetCurrentMapBrushModel()
{
    std::string strMapName = Dvar_GetString("ui_mapname");

    if (strMapName == "mp_afghan")
        return (gentity_s*)0x82F7E800;
    if (strMapName == "mp_derail")
        return (gentity_s*)0x82F5F680;
    if (strMapName == "mp_estate")
        return (gentity_s*)0x82F4AF00;
    if (strMapName == "mp_favela")
        return (gentity_s*)0x82F70C00;
    if (strMapName == "mp_highrise")
        return (gentity_s*)0x82F76380;
    if (strMapName == "mp_invasion")
        return (gentity_s*)0x82F73180;
    if (strMapName == "mp_checkpoint")
        return (gentity_s*)0x82F6D280;
    if (strMapName == "mp_quarry")
        return (gentity_s*)0x82F99600;
    if (strMapName == "mp_rundown")
        return (gentity_s*)0x82F65800;
    if (strMapName == "mp_rust")
        return (gentity_s*)0x82F3C900;
    if (strMapName == "mp_boneyard")
        return (gentity_s*)0x82F0B300;
    if (strMapName == "mp_nightshift")
        return (gentity_s*)0x82F3A880;
    if (strMapName == "mp_subbase")
        return (gentity_s*)0x82F59780;
    if (strMapName == "mp_terminal")
        return (gentity_s*)0x82F0DD80;
    if (strMapName == "mp_underpass")
        return (gentity_s*)0x82F66C00;
    if (strMapName == "mp_brecourt")
        return (gentity_s*)0x82F15580;
    if (strMapName == "mp_complex")
        return (gentity_s*)0x82F09F00;
    if (strMapName == "mp_crash")
        return (gentity_s*)0x82F18280;
    if (strMapName == "mp_overgrown")
        return (gentity_s*)0x82F34980;
    if (strMapName == "mp_compact")
        return (gentity_s*)0x82F14180;
    if (strMapName == "mp_storm")
        return (gentity_s*)0x82F5DB00;
    if (strMapName == "mp_abandon")
        return (gentity_s*)0x82F48980;
    if (strMapName == "mp_fuel2")
        return (gentity_s*)0x82F65300;
    if (strMapName == "mp_strike")
        return (gentity_s*)0x82F4D980;
    if (strMapName == "mp_trailerpark")
        return (gentity_s*)0x82F17380;
    if (strMapName == "mp_vacant")
        return (gentity_s*)0x82F55900;
    else
        return nullptr;
}

}
