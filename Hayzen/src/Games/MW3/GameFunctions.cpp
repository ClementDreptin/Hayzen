#include "pch.h"
#include "Games\MW3\GameFunctions.h"


namespace MW3GameFunctions
{

static std::unordered_map<std::string, DWORD> BrushModelMap;


LPCSTR (*SL_ConvertToString)(UINT stringValue) = reinterpret_cast<LPCSTR(*)(UINT)>(0x822B5120);

LPCSTR (*Dvar_GetString)(LPCSTR dvarName) = reinterpret_cast<LPCSTR(*)(LPCSTR)>(0x8232E488);

clientState_s *(*GetClientState)(INT clientNum) = reinterpret_cast<clientState_s *(*)(INT)>(0x82244130);

playerState_s *(*GetPlayerState)(INT clientNum) = reinterpret_cast<playerState_s *(*)(INT)>(0x82244148);

bool (*Session_IsHost)(DWORD sessionDataPtr, INT clientNum) = reinterpret_cast<bool(*)(DWORD, INT)>(0x823BFE78);

VOID (*SP_script_model)(gentity_s *mSelf) = reinterpret_cast<VOID(*)(gentity_s *)>(0x82268138);

gentity_s*(*G_Spawn)() = reinterpret_cast<gentity_s *(*)()>(0x8226F378);

VOID (*G_SetModel)(gentity_s *ent, LPCSTR modelName) = reinterpret_cast<VOID(*)(gentity_s *, LPCSTR)>(0x8226EAA0);

VOID (*SV_LinkEntity)(gentity_s *gEnt) = reinterpret_cast<VOID(*)(gentity_s *)>(0x822D6E78);

VOID (*SV_UnlinkEntity)(gentity_s *gEnt) = reinterpret_cast<VOID(*)(gentity_s *)>(0x822D6D90);

VOID (*SV_SetBrushModel)(gentity_s *ent) = reinterpret_cast<VOID(*)(gentity_s *)>(0x822C95C8);

VOID (*TeleportPlayer)(gentity_s *player, PFLOAT origin, PFLOAT angles) = reinterpret_cast<VOID(*)(gentity_s *, PFLOAT, PFLOAT)>(0x82246048);

gclient_s *GetGClient(INT clientNum)
{
    return reinterpret_cast<gclient_s *>(0x82F99580 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(INT entNum)
{
    return reinterpret_cast<gentity_s *>(0x82DCCC80 + sizeof(gentity_s) * entNum);
}

bool IsHost(INT clientNum)
{
    return Session_IsHost(0x83BC0148, clientNum);
}

static VOID InitBrushModelMap()
{
    BrushModelMap["mp_seatown"] = 0x82DD1280;
    BrushModelMap["mp_mogadishu"] = 0x82E08A00;
    BrushModelMap["mp_exchange"] = 0x82E14580;
    BrushModelMap["mp_radar"] = 0x82DD3A80;
    BrushModelMap["mp_terminal_cls"] = 0x82DF9C80;
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

    gentity_s *pBrushModel = reinterpret_cast<gentity_s *>(BrushModelMap[strMapName]);
    if (!pBrushModel)
        pBrushModel = reinterpret_cast<gentity_s *>(0x82DD1500);

    return pBrushModel;
}

}
