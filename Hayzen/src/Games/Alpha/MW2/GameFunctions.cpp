#include "pch.h"
#include "Games\Alpha\MW2\GameFunctions.h"


namespace AlphaMW2GameFunctions
{

static std::unordered_map<std::string, DWORD> BrushModelMap;


LPCSTR (*SL_ConvertToString)(UINT stringValue) = reinterpret_cast<LPCSTR(*)(UINT)>(0x8229A730);

VOID (*Cbuf_AddText)(INT localClientNum, LPCSTR text) = reinterpret_cast<VOID(*)(INT, LPCSTR)>(0x8226F590);

bool (*Dvar_GetBool)(LPCSTR dvarName) = reinterpret_cast<bool(*)(LPCSTR)>(0x82303B00);

FLOAT (*Dvar_GetFloat)(LPCSTR dvarName) = reinterpret_cast<FLOAT(*)(LPCSTR)>(0x82303BD0);

LPCSTR (*Dvar_GetString)(LPCSTR dvarName) = reinterpret_cast<LPCSTR(*)(LPCSTR)>(0x82303CC0);

playerState_s *(*GetPlayerState)(INT clientNum) = reinterpret_cast<playerState_s *(*)(INT)>(0x8222C108);

bool (*Session_IsHost)(DWORD sessionDataPtr, INT clientNum) = reinterpret_cast<bool(*)(DWORD, INT)>(0x82388338);

VOID (*SP_script_model)(gentity_s *mSelf) = reinterpret_cast<VOID(*)(gentity_s *)>(0x82250A20);

gentity_s *(*G_Spawn)() = reinterpret_cast<gentity_s *(*)()>(0x82258508);

VOID (*G_SetModel)(gentity_s *ent, LPCSTR modelName) = reinterpret_cast<VOID(*)(gentity_s *, LPCSTR)>(0x82257B90);

VOID (*SV_LinkEntity)(gentity_s *gEnt) = reinterpret_cast<VOID(*)(gentity_s *)>(0x822C1598);

VOID (*SV_UnlinkEntity)(gentity_s *gEnt) = reinterpret_cast<VOID(*)(gentity_s *)>(0x822C14B0);

VOID (*SV_SetBrushModel)(gentity_s *ent) = reinterpret_cast<VOID(*)(gentity_s *)>(0x822B63B8);

gentity_s *(*SV_AddTestClient)() = reinterpret_cast<gentity_s *(*)()>(0x822B5F00);

VOID (*SV_ExecuteClientCommand)(INT client, LPCSTR s, INT clientOK, INT fromOldServer) = reinterpret_cast<VOID(*)(INT, LPCSTR, INT, INT)>(0x822B4700);

VOID (*TeleportPlayer)(gentity_s *player, PFLOAT origin, PFLOAT angles) = reinterpret_cast<VOID(*)(gentity_s *, PFLOAT, PFLOAT)>(0x8222E5A0);

gclient_s *GetGClient(INT clientNum)
{
    return reinterpret_cast<gclient_s *>(0x82F01480 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(INT entNum)
{
    return reinterpret_cast<gentity_s *>(0x82D47D80 + sizeof(gentity_s) * entNum);
}

bool IsHost(INT clientNum)
{
    return Session_IsHost(0x83A06F28, clientNum);
}

static VOID InitBrushModelMap()
{
    BrushModelMap["mp_afghan"] = 0x82D60880;
    BrushModelMap["mp_checkpoint"] = 0x82D76B80;
    BrushModelMap["mp_derail"] = 0x82D95800;
    BrushModelMap["mp_estate"] = 0x82D5B600;
    BrushModelMap["mp_favela"] = 0x82D9A300;
    BrushModelMap["mp_highrise"] = 0x82DC4380;
    BrushModelMap["mp_invasion"] = 0x82D7E100;
    BrushModelMap["mp_quarry"] = 0x82D70F00;
    BrushModelMap["mp_rundown"] = 0x82D93780;
    BrushModelMap["mp_rust"] = 0x82D5A980;
    BrushModelMap["mp_boneyard"] = 0x82D64E80;
    BrushModelMap["mp_nightshift"] = 0x82D58180;
    BrushModelMap["mp_subbase"] = 0x82D7CF80;
    BrushModelMap["mp_terminal"] = 0x82D61A00;
    BrushModelMap["mp_underpass"] = 0x82D52000;
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
