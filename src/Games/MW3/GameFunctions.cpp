#include "pch.h"
#include "Games/MW3/GameFunctions.h"

namespace MW3
{
namespace Game
{

static std::unordered_map<std::string, uintptr_t> brushModelMap;

const char *(*SL_ConvertToString)(uint32_t stringValue) = reinterpret_cast<const char *(*)(uint32_t)>(0x822B5120);

void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text) = reinterpret_cast<void (*)(int, int, const char *)>(0x822C9340);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<void (*)(int, const char *)>(0x82287F68);

const char *(*Dvar_GetString)(const char *dvarName) = reinterpret_cast<const char *(*)(const char *)>(0x8232E488);

clientState_s *(*GetClientState)(int clientNum) = reinterpret_cast<clientState_s *(*)(int)>(0x82244130);

playerState_s *(*GetPlayerState)(int clientNum) = reinterpret_cast<playerState_s *(*)(int)>(0x82244148);

bool (*Session_IsHost)(uintptr_t sessionDataPtr, int clientNum) = reinterpret_cast<bool (*)(uintptr_t, int)>(0x823BFE78);

void (*SP_script_model)(gentity_s *mSelf) = reinterpret_cast<void (*)(gentity_s *)>(0x82268138);

gentity_s *(*G_Spawn)() = reinterpret_cast<gentity_s *(*)()>(0x8226F378);

void (*G_SetModel)(gentity_s *ent, const char *modelName) = reinterpret_cast<void (*)(gentity_s *, const char *)>(0x8226EAA0);

void (*SV_LinkEntity)(gentity_s *gEnt) = reinterpret_cast<void (*)(gentity_s *)>(0x822D6E78);

void (*SV_UnlinkEntity)(gentity_s *gEnt) = reinterpret_cast<void (*)(gentity_s *)>(0x822D6D90);

bool (*SV_SetBrushModel)(gentity_s *ent) = reinterpret_cast<bool (*)(gentity_s *)>(0x822C95C8);

void (*SetClientOrigin)(gentity_s *ent, const float *origin) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x8222FF50);

void (*SetClientViewAngle)(gentity_s *ent, const float *angle) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x8222FD10);

void iPrintLn(int clientNum, const std::string &text)
{
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    return reinterpret_cast<gclient_s *>(0x82F99580 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(int entNum)
{
    return reinterpret_cast<gentity_s *>(0x82DCCC80 + sizeof(gentity_s) * entNum);
}

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value)
{
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("q %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(int clientNum)
{
    return Session_IsHost(0x83BC0148, clientNum);
}

static void InitBrushModelMap()
{
    brushModelMap["mp_seatown"] = 0x82DD1280;
    brushModelMap["mp_mogadishu"] = 0x82E08A00;
    brushModelMap["mp_exchange"] = 0x82E14580;
    brushModelMap["mp_radar"] = 0x82DD3A80;
    brushModelMap["mp_terminal_cls"] = 0x82DF9C80;
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
        pBrushModel = reinterpret_cast<gentity_s *>(0x82DD1500);

    return pBrushModel;
}

}
}
