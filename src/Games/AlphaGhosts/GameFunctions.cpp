#include "pch.h"
#include "Games/AlphaGhosts/GameFunctions.h"

namespace AlphaGhosts
{
namespace Game
{

static std::unordered_map<std::string, uintptr_t> brushModelMap;

const char *(*SL_ConvertToString)(uint32_t stringValue) = reinterpret_cast<const char *(*)(uint32_t)>(0x826F90B8);

void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text) = reinterpret_cast<void (*)(int, int, const char *)>(0x82769058);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<void (*)(int, const char *)>(0x8266A130);

bool (*Dvar_GetBool)(const char *dvarName) = reinterpret_cast<bool (*)(const char *)>(0x82832AC8);

const char *(*Dvar_GetString)(const char *dvarName) = reinterpret_cast<const char *(*)(const char *)>(0x82832D20);

playerState_s *(*GetPlayerState)(int clientNum) = reinterpret_cast<playerState_s *(*)(int)>(0x825FE810);

bool (*Session_IsHost)(uintptr_t sessionDataPtr, int clientNum) = reinterpret_cast<bool (*)(uintptr_t, int)>(0x82914CE8);

void (*SP_script_model)(gentity_s *mSelf) = reinterpret_cast<void (*)(gentity_s *)>(0x82638078);

gentity_s *(*G_Spawn)() = reinterpret_cast<gentity_s *(*)()>(0x826410A0);

void (*G_SetModel)(gentity_s *ent, const char *modelName) = reinterpret_cast<void (*)(gentity_s *, const char *)>(0x82642428);

void (*SV_LinkEntity)(gentity_s *gEnt) = reinterpret_cast<void (*)(gentity_s *)>(0x8278BB40);

void (*SV_UnlinkEntity)(gentity_s *gEnt) = reinterpret_cast<void (*)(gentity_s *)>(0x8278B6D0);

bool (*SV_SetBrushModel)(gentity_s *ent) = reinterpret_cast<bool (*)(gentity_s *)>(0x82769280);

void (*SetClientOrigin)(gentity_s *ent, const float *origin) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x825DD288);

void (*SetClientViewAngle)(gentity_s *ent, const float *angle) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x825DCFF8);

bool (*UI_AnyMenuActive)(int localClientNum) = reinterpret_cast<bool (*)(int)>(0x827CD838);

void iPrintLn(int clientNum, const std::string &text)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    // This is what iPrintLnBold does technically but iPrintLn doesn't work, I don't know why...
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("g \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    return reinterpret_cast<gclient_s *>(0x8385E700 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(uint32_t entNum)
{
    return reinterpret_cast<gentity_s *>(0x8390E000 + sizeof(gentity_s) * entNum);
}

bool IsHost(int clientNum)
{
    if (clientNum < 0 || clientNum > 17)
        return false;

    return Session_IsHost(0x84A8B458, clientNum);
}

static void InitBrushModelMap()
{
    // The maps set to nullptr don't work and there's a comment with the error

    brushModelMap["mp_dome"] = 0x8391CB00;
    brushModelMap["mp_plaza2"] = 0x83918000;
    brushModelMap["mp_paris"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    brushModelMap["mp_lonestar"] = 0x83927280;
    brushModelMap["mp_frag"] = 0x8391A800;
    brushModelMap["mp_snow"] = 0x83919180;
    brushModelMap["mp_fahrenheit"] = 0x83924800;
    brushModelMap["mp_hasima"] = reinterpret_cast<uintptr_t>(nullptr); // disc unreadable
    brushModelMap["mp_warhawk"] = 0x83917D80;
    brushModelMap["mp_sovereign"] = 0x83919400;
    brushModelMap["mp_zebra"] = 0x83918780;
}

gentity_s *GetCurrentMapBrushModel()
{
    static bool isBrushModelMapInitialized = false;

    if (!isBrushModelMapInitialized)
    {
        InitBrushModelMap();
        isBrushModelMapInitialized = true;
    }

    std::string mapName = Dvar_GetString("ui_mapname");

    gentity_s *pBrushModel = reinterpret_cast<gentity_s *>(brushModelMap[mapName]);
    if (!pBrushModel)
        pBrushModel = reinterpret_cast<gentity_s *>(0x83917B00);

    return pBrushModel;
}

}
}
