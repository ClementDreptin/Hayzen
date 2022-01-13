#include "pch.h"
#include "Games\MW3\GameFunctions.h"


namespace MW3GameFunctions
{

static std::unordered_map<std::string, DWORD> BrushModelMap;


const char *(*SL_ConvertToString)(uint32_t stringValue) = reinterpret_cast<const char *(*)(uint32_t)>(0x822B5120);

const char *(*Dvar_GetString)(const char *dvarName) = reinterpret_cast<const char *(*)(const char *)>(0x8232E488);

clientState_s *(*GetClientState)(int clientNum) = reinterpret_cast<clientState_s *(*)(int)>(0x82244130);

playerState_s *(*GetPlayerState)(int clientNum) = reinterpret_cast<playerState_s *(*)(int)>(0x82244148);

bool (*Session_IsHost)(DWORD sessionDataPtr, int clientNum) = reinterpret_cast<bool(*)(DWORD, int)>(0x823BFE78);

void (*SP_script_model)(gentity_s *mSelf) = reinterpret_cast<void(*)(gentity_s *)>(0x82268138);

gentity_s*(*G_Spawn)() = reinterpret_cast<gentity_s *(*)()>(0x8226F378);

void (*G_SetModel)(gentity_s *ent, const char *modelName) = reinterpret_cast<void(*)(gentity_s *, const char *)>(0x8226EAA0);

void (*SV_LinkEntity)(gentity_s *gEnt) = reinterpret_cast<void(*)(gentity_s *)>(0x822D6E78);

void (*SV_UnlinkEntity)(gentity_s *gEnt) = reinterpret_cast<void(*)(gentity_s *)>(0x822D6D90);

void (*SV_SetBrushModel)(gentity_s *ent) = reinterpret_cast<void(*)(gentity_s *)>(0x822C95C8);

void (*TeleportPlayer)(gentity_s *player, float *origin, float *angles) = reinterpret_cast<void(*)(gentity_s *, float *, float *)>(0x82246048);

gclient_s *GetGClient(int clientNum)
{
    return reinterpret_cast<gclient_s *>(0x82F99580 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(int entNum)
{
    return reinterpret_cast<gentity_s *>(0x82DCCC80 + sizeof(gentity_s) * entNum);
}

bool IsHost(int clientNum)
{
    return Session_IsHost(0x83BC0148, clientNum);
}

static void InitBrushModelMap()
{
    BrushModelMap["mp_seatown"] = 0x82DD1280;
    BrushModelMap["mp_mogadishu"] = 0x82E08A00;
    BrushModelMap["mp_exchange"] = 0x82E14580;
    BrushModelMap["mp_radar"] = 0x82DD3A80;
    BrushModelMap["mp_terminal_cls"] = 0x82DF9C80;
}

gentity_s *GetCurrentMapBrushModel()
{
    static bool bBrushModelMapInitialized = false;

    if (!bBrushModelMapInitialized)
    {
        InitBrushModelMap();
        bBrushModelMapInitialized = true;
    }

    std::string strMapName = Dvar_GetString("ui_mapname");

    gentity_s *pBrushModel = reinterpret_cast<gentity_s *>(BrushModelMap[strMapName]);
    if (!pBrushModel)
        pBrushModel = reinterpret_cast<gentity_s *>(0x82DD1500);

    return pBrushModel;
}

}
