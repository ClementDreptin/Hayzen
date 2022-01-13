#include "pch.h"
#include "Games\MW2\GameFunctions.h"


namespace MW2GameFunctions
{

static std::unordered_map<std::string, DWORD> BrushModelMap;


LPCSTR (*SL_ConvertToString)(UINT stringValue) = reinterpret_cast<LPCSTR(*)(UINT)>(0x82241898);

VOID (*SV_GameSendServerCommand)(INT clientNum, INT type, LPCSTR text) = reinterpret_cast<VOID(*)(INT, INT, LPCSTR)>(0x822548D8);

bool (*Dvar_GetBool)(LPCSTR dvarName) = reinterpret_cast<bool(*)(LPCSTR)>(0x8229EEE8);

LPCSTR (*Dvar_GetString)(LPCSTR dvarName) = reinterpret_cast<LPCSTR(*)(LPCSTR)>(0x8229F0A8);

clientState_s *(*GetClientState)(INT clientNum) = reinterpret_cast<clientState_s *(*)(INT)>(0x821E6610);

playerState_s *(*GetPlayerState)(INT clientNum) = reinterpret_cast<playerState_s *(*)(INT)>(0x821E6628);

bool (*Session_IsHost)(DWORD sessionDataPtr, INT clientNum) = reinterpret_cast<bool(*)(DWORD, INT)>(0x82320138);

VOID (*SP_script_model)(gentity_s *mSelf) = reinterpret_cast<VOID(*)(gentity_s *)>(0x82206D88);

gentity_s*(*G_Spawn)() = reinterpret_cast<gentity_s *(*)()>(0x8220DB50);

VOID (*G_SetModel)(gentity_s *ent, LPCSTR modelName) = reinterpret_cast<VOID(*)(gentity_s *, LPCSTR)>(0x8220D278);

VOID (*SV_LinkEntity)(gentity_s *gEnt) = reinterpret_cast<VOID(*)(gentity_s *)>(0x8225F518);

VOID (*SV_UnlinkEntity)(gentity_s *gEnt) = reinterpret_cast<VOID(*)(gentity_s *)>(0x8225F430);

VOID (*SV_SetBrushModel)(gentity_s *ent) = reinterpret_cast<VOID(*)(gentity_s *)>(0x82254B50);

gentity_s *(*SV_AddTestClient)() = reinterpret_cast<gentity_s *(*)()>(0x82254690);

VOID (*SV_ExecuteClientCommand)(INT client, LPCSTR s, INT clientOK, INT fromOldServer) = reinterpret_cast<VOID(*)(INT, LPCSTR, INT, INT)>(0x82253140);

VOID (*TeleportPlayer)(gentity_s *player, PFLOAT origin, PFLOAT angles) = reinterpret_cast<VOID(*)(gentity_s *, PFLOAT, PFLOAT)>(0x821E8198);

gclient_s *GetGClient(INT clientNum)
{
    return reinterpret_cast<gclient_s *>(0x830CBF80 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(INT entNum)
{
    return reinterpret_cast<gentity_s *>(0x82F03600 + sizeof(gentity_s) * entNum);
}

VOID SetClientDvar(INT clientNum, CONST std::string &dvar, CONST std::string &value)
{
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("s %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(INT clientNum)
{
    return Session_IsHost(0x83AC3DB0, clientNum);
}

static VOID InitBrushModelMap()
{
    BrushModelMap["mp_afghan"] = 0x82F7E800;
    BrushModelMap["mp_derail"] = 0x82F5F680;
    BrushModelMap["mp_estate"] = 0x82F4AF00;
    BrushModelMap["mp_favela"] = 0x82F70C00;
    BrushModelMap["mp_highrise"] = 0x82F76380;
    BrushModelMap["mp_invasion"] = 0x82F73180;
    BrushModelMap["mp_checkpoint"] = 0x82F6D280;
    BrushModelMap["mp_quarry"] = 0x82F99600;
    BrushModelMap["mp_rundown"] = 0x82F65800;
    BrushModelMap["mp_rust"] = 0x82F3C900;
    BrushModelMap["mp_boneyard"] = 0x82F0B300;
    BrushModelMap["mp_nightshift"] = 0x82F3A880;
    BrushModelMap["mp_subbase"] = 0x82F59780;
    BrushModelMap["mp_terminal"] = 0x82F0DD80;
    BrushModelMap["mp_underpass"] = 0x82F66C00;
    BrushModelMap["mp_brecourt"] = 0x82F15580;
    BrushModelMap["mp_complex"] = 0x82F09F00;
    BrushModelMap["mp_crash"] = 0x82F18280;
    BrushModelMap["mp_overgrown"] = 0x82F34980;
    BrushModelMap["mp_compact"] = 0x82F14180;
    BrushModelMap["mp_storm"] = 0x82F5DB00;
    BrushModelMap["mp_abandon"] = 0x82F48980;
    BrushModelMap["mp_fuel2"] = 0x82F65300;
    BrushModelMap["mp_strike"] = 0x82F4D980;
    BrushModelMap["mp_trailerpark"] = 0x82F17380;
    BrushModelMap["mp_vacant"] = 0x82F55900;
}

gentity_s *GetCurrentMapBrushModel()
{
    static BOOL bBrushModelMapInitialized = FALSE;

    if (!bBrushModelMapInitialized)
    {
        InitBrushModelMap();
        bBrushModelMapInitialized = TRUE;
    }

    std::string strMapName = Dvar_GetString("ui_mapname");

    return reinterpret_cast<gentity_s *>(BrushModelMap[strMapName]);
}

}
