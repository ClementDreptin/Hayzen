#include "pch.h"
#include "Games/MW2/GameFunctions.h"

namespace MW2
{
namespace Game
{

static std::unordered_map<std::string, uintptr_t> brushModelMap;

const char *(*SL_ConvertToString)(uint32_t stringValue) = reinterpret_cast<const char *(*)(uint32_t)>(0x82241898);

void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text) = reinterpret_cast<void (*)(int, int, const char *)>(0x822548D8);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<void (*)(int, const char *)>(0x82224990);

bool (*Dvar_GetBool)(const char *dvarName) = reinterpret_cast<bool (*)(const char *)>(0x8229EEE8);

const char *(*Dvar_GetString)(const char *dvarName) = reinterpret_cast<const char *(*)(const char *)>(0x8229F0A8);

clientState_s *(*GetClientState)(int clientNum) = reinterpret_cast<clientState_s *(*)(int)>(0x821E6610);

playerState_s *(*GetPlayerState)(int clientNum) = reinterpret_cast<playerState_s *(*)(int)>(0x821E6628);

bool (*Session_IsHost)(uintptr_t sessionDataPtr, int clientNum) = reinterpret_cast<bool (*)(uintptr_t, int)>(0x82320138);

void (*SP_script_model)(gentity_s *mSelf) = reinterpret_cast<void (*)(gentity_s *)>(0x82206D88);

gentity_s *(*G_Spawn)() = reinterpret_cast<gentity_s *(*)()>(0x8220DB50);

void (*G_SetModel)(gentity_s *ent, const char *modelName) = reinterpret_cast<void (*)(gentity_s *, const char *)>(0x8220D278);

void (*SV_LinkEntity)(gentity_s *gEnt) = reinterpret_cast<void (*)(gentity_s *)>(0x8225F518);

void (*SV_UnlinkEntity)(gentity_s *gEnt) = reinterpret_cast<void (*)(gentity_s *)>(0x8225F430);

void (*SV_SetBrushModel)(gentity_s *ent) = reinterpret_cast<void (*)(gentity_s *)>(0x82254B50);

gentity_s *(*SV_AddTestClient)() = reinterpret_cast<gentity_s *(*)()>(0x82254690);

void (*SV_ExecuteClientCommand)(int client, const char *s, int clientOK, int fromOldServer) = reinterpret_cast<void (*)(int, const char *, int, int)>(0x82253140);

void (*SetClientOrigin)(gentity_s *ent, const float *origin) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x821D3798);

void (*SetClientViewAngle)(gentity_s *ent, const float *angle) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x821D3550);

void iPrintLn(int clientNum, const std::string &text)
{
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    return reinterpret_cast<gclient_s *>(0x830CBF80 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(int entNum)
{
    return reinterpret_cast<gentity_s *>(0x82F03600 + sizeof(gentity_s) * entNum);
}

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value)
{
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("s %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(int clientNum)
{
    return Session_IsHost(0x83AC3DB0, clientNum);
}

static void InitBrushModelMap()
{
    brushModelMap["mp_afghan"] = 0x82F7E800;
    brushModelMap["mp_derail"] = 0x82F5F680;
    brushModelMap["mp_estate"] = 0x82F4AF00;
    brushModelMap["mp_favela"] = 0x82F70C00;
    brushModelMap["mp_highrise"] = 0x82F76380;
    brushModelMap["mp_invasion"] = 0x82F73180;
    brushModelMap["mp_checkpoint"] = 0x82F6D280;
    brushModelMap["mp_quarry"] = 0x82F99600;
    brushModelMap["mp_rundown"] = 0x82F65800;
    brushModelMap["mp_rust"] = 0x82F3C900;
    brushModelMap["mp_boneyard"] = 0x82F0B300;
    brushModelMap["mp_nightshift"] = 0x82F3A880;
    brushModelMap["mp_subbase"] = 0x82F59780;
    brushModelMap["mp_terminal"] = 0x82F0DD80;
    brushModelMap["mp_underpass"] = 0x82F66C00;
    brushModelMap["mp_brecourt"] = 0x82F15580;
    brushModelMap["mp_complex"] = 0x82F09F00;
    brushModelMap["mp_crash"] = 0x82F18280;
    brushModelMap["mp_overgrown"] = 0x82F34980;
    brushModelMap["mp_compact"] = 0x82F14180;
    brushModelMap["mp_storm"] = 0x82F5DB00;
    brushModelMap["mp_abandon"] = 0x82F48980;
    brushModelMap["mp_fuel2"] = 0x82F65300;
    brushModelMap["mp_strike"] = 0x82F4D980;
    brushModelMap["mp_trailerpark"] = 0x82F17380;
    brushModelMap["mp_vacant"] = 0x82F55900;
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

    return reinterpret_cast<gentity_s *>(brushModelMap[mapName]);
}

}
}
