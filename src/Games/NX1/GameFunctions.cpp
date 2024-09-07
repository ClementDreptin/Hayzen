#include "pch.h"
#include "Games/NX1/GameFunctions.h"

namespace NX1
{
namespace Game
{

static std::unordered_map<std::string, uintptr_t> brushModelMap;

const char *(*SL_ConvertToString)(uint32_t stringValue) = reinterpret_cast<const char *(*)(uint32_t)>(0x823024E8);

void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text) = reinterpret_cast<void (*)(int, int, const char *)>(0x8231D1D8);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<void (*)(int, const char *)>(0x822C8C20);

const char *(*Dvar_GetString)(const char *dvarName) = reinterpret_cast<const char *(*)(const char *)>(0x82374AA0);

playerState_s *(*GetPlayerState)(int clientNum) = reinterpret_cast<playerState_s *(*)(int)>(0x8227E2A0);

bool (*Session_IsHost)(uintptr_t sessionDataPtr, int clientNum) = reinterpret_cast<bool (*)(uintptr_t, int)>(0x82401628);

void (*SP_script_model)(gentity_s *mSelf) = reinterpret_cast<void (*)(gentity_s *)>(0x822A7500);

gentity_s *(*G_Spawn)() = reinterpret_cast<gentity_s *(*)()>(0x822AF990);

void (*G_SetModel)(gentity_s *ent, const char *modelName) = reinterpret_cast<void (*)(gentity_s *, const char *)>(0x822AEFB0);

void (*SV_LinkEntity)(gentity_s *gEnt) = reinterpret_cast<void (*)(gentity_s *)>(0x82329918);

void (*SV_UnlinkEntity)(gentity_s *gEnt) = reinterpret_cast<void (*)(gentity_s *)>(0x82329850);

bool (*SV_SetBrushModel)(gentity_s *ent) = reinterpret_cast<bool (*)(gentity_s *)>(0x8231D460);

gentity_s *(*SV_AddTestClient)() = reinterpret_cast<gentity_s *(*)()>(0x8231CF18);

void (*SV_ExecuteClientCommand)(client_t *client, const char *s, int clientOK, int fromOldServer) = reinterpret_cast<void (*)(client_t *, const char *, int, int)>(0x8231B8E8);

void (*SetClientOrigin)(gentity_s *ent, const float *origin) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x82264A20);

void (*SetClientViewAngle)(gentity_s *ent, const float *angle) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x82264770);

void iPrintLn(int clientNum, const std::string &text)
{
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    return reinterpret_cast<gclient_s *>(0x83362C80 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(int entNum)
{
    return reinterpret_cast<gentity_s *>(0x83147800 + sizeof(gentity_s) * entNum);
}

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value)
{
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("v %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(int clientNum)
{
    return Session_IsHost(0x83F0E758, clientNum);
}

static void InitBrushModelMap()
{
    // The maps set to nullptr don't work and there's a comment with the error

    brushModelMap["mp_nx_pitstop"] = 0x83168E00;
    brushModelMap["mp_nx_galleria"] = 0x83169100;
    brushModelMap["mp_nx_stasis"] = 0x83160A00;
    brushModelMap["mp_nx_fallout"] = 0x83155900;
    brushModelMap["mp_nx_meteor"] = 0x83156500;
    brushModelMap["mp_nx_border"] = 0x83160A00;
    brushModelMap["mp_nx_lockdown_v2"] = 0x83155900;
    brushModelMap["mp_nx_contact"] = 0x83166100;
    brushModelMap["mp_nx_ugvcontact"] = 0x8315D100;
    brushModelMap["mp_nx_ugvhh"] = 0x83175400;
    brushModelMap["mp_nx_asylum"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    brushModelMap["mp_nx_asylum_2"] = 0x83155F00;
    brushModelMap["mp_nx_bom"] = 0x83163A00;
    brushModelMap["mp_nx_deadzone"] = 0x8315AA00;
    brushModelMap["mp_nx_dust"] = reinterpret_cast<uintptr_t>(nullptr);      // missing zone
    brushModelMap["mp_nx_frontline"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    brushModelMap["mp_nx_import"] = 0x83156500;
    brushModelMap["mp_nx_lunar"] = reinterpret_cast<uintptr_t>(nullptr);    // missing zone
    brushModelMap["mp_nx_memorial"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    brushModelMap["mp_nx_monorail"] = 0x8319BE00;
    brushModelMap["mp_nx_sandstorm"] = 0x8315FB00;
    brushModelMap["mp_nx_seaport"] = 0x83150B00;
    brushModelMap["mp_nx_skylab"] = 0x83155600;
    brushModelMap["mp_nx_skylight"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    brushModelMap["mp_nx_streets"] = 0x83159E00;
    brushModelMap["mp_nx_subyard"] = 0x8314E400;
    brushModelMap["mp_nx_ugvsand"] = 0x83179600;
    brushModelMap["mp_nx_whiteout"] = 0x83185F00;
    brushModelMap["mp_nx_leg_afghan"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    brushModelMap["mp_nx_leg_crash"] = 0x83161600;
    brushModelMap["mp_nx_leg_outpost"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    brushModelMap["mp_nx_leg_over"] = 0x83182900;
    brushModelMap["mp_nx_leg_term"] = 0x83154100;
    brushModelMap["mp_nx_apt"] = reinterpret_cast<uintptr_t>(nullptr);          // missing zone
    brushModelMap["mp_nx_binscrib"] = reinterpret_cast<uintptr_t>(nullptr);     // missing zone
    brushModelMap["mp_nx_g_assault"] = reinterpret_cast<uintptr_t>(nullptr);    // missing zone
    brushModelMap["mp_nx_merc_over"] = reinterpret_cast<uintptr_t>(nullptr);    // missing zone
    brushModelMap["mp_nx_mercmode"] = reinterpret_cast<uintptr_t>(nullptr);     // missing zone
    brushModelMap["mp_nx_test_benk_04"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    brushModelMap["mp_nx_blanktest"] = reinterpret_cast<uintptr_t>(nullptr);    // disc unreadable
    brushModelMap["mp_nx_jku"] = reinterpret_cast<uintptr_t>(nullptr);          // missing zone
    brushModelMap["mp_nx_jpldock"] = reinterpret_cast<uintptr_t>(nullptr);      // missing zone
    brushModelMap["mp_nx_milota1"] = reinterpret_cast<uintptr_t>(nullptr);      // missing zone
    brushModelMap["mp_nx_testmap"] = reinterpret_cast<uintptr_t>(nullptr);      // missing zone
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
