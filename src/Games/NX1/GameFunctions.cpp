#include "pch.h"
#include "Games/NX1/GameFunctions.h"

namespace NX1
{
namespace Game
{

static std::unordered_map<std::string, uintptr_t> s_CrateBrushModelMap;

decltype(SL_ConvertToString) SL_ConvertToString = reinterpret_cast<decltype(SL_ConvertToString)>(0x823024E8);

decltype(SV_GameSendServerCommand) SV_GameSendServerCommand = reinterpret_cast<decltype(SV_GameSendServerCommand)>(0x8231D1D8);

decltype(Cbuf_AddText) Cbuf_AddText = reinterpret_cast<decltype(Cbuf_AddText)>(0x822C8C20);

decltype(Dvar_GetString) Dvar_GetString = reinterpret_cast<decltype(Dvar_GetString)>(0x82374AA0);

decltype(Dvar_GetFloat) Dvar_GetFloat = reinterpret_cast<decltype(Dvar_GetFloat)>(0x823749B0);

decltype(GetPlayerState) GetPlayerState = reinterpret_cast<decltype(GetPlayerState)>(0x8227E2A0);

decltype(Session_IsHost) Session_IsHost = reinterpret_cast<decltype(Session_IsHost)>(0x82401628);

decltype(SP_script_model) SP_script_model = reinterpret_cast<decltype(SP_script_model)>(0x822A7500);

decltype(G_Spawn) G_Spawn = reinterpret_cast<decltype(G_Spawn)>(0x822AF990);

decltype(G_SetModel) G_SetModel = reinterpret_cast<decltype(G_SetModel)>(0x822AEFB0);

decltype(SV_LinkEntity) SV_LinkEntity = reinterpret_cast<decltype(SV_LinkEntity)>(0x82329918);

decltype(SV_UnlinkEntity) SV_UnlinkEntity = reinterpret_cast<decltype(SV_UnlinkEntity)>(0x82329850);

decltype(SV_SetBrushModel) SV_SetBrushModel = reinterpret_cast<decltype(SV_SetBrushModel)>(0x8231D460);

decltype(SV_AddTestClient) SV_AddTestClient = reinterpret_cast<decltype(SV_AddTestClient)>(0x8231CF18);

decltype(SV_ExecuteClientCommand) SV_ExecuteClientCommand = reinterpret_cast<decltype(SV_ExecuteClientCommand)>(0x8231B8E8);

decltype(SetClientOrigin) SetClientOrigin = reinterpret_cast<decltype(SetClientOrigin)>(0x82264A20);

decltype(SetClientViewAngle) SetClientViewAngle = reinterpret_cast<decltype(SetClientViewAngle)>(0x82264770);

decltype(UI_AnyMenuActive) UI_AnyMenuActive = reinterpret_cast<decltype(UI_AnyMenuActive)>(0x8233F188);

void iPrintLn(int clientNum, const std::string &text)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    return reinterpret_cast<gclient_s *>(0x83362C80 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(uint32_t entNum)
{
    return reinterpret_cast<gentity_s *>(0x83147800 + sizeof(gentity_s) * entNum);
}

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("v %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(int clientNum)
{
    if (clientNum < 0 || clientNum > 17)
        return false;

    return Session_IsHost(0x83F0E758, clientNum);
}

static void InitBrushModelMap()
{
    // The maps set to nullptr don't work and there's a comment with the error

    s_CrateBrushModelMap["mp_nx_pitstop"] = 0x83168E00;
    s_CrateBrushModelMap["mp_nx_galleria"] = 0x83169100;
    s_CrateBrushModelMap["mp_nx_stasis"] = 0x83160A00;
    s_CrateBrushModelMap["mp_nx_fallout"] = 0x83155900;
    s_CrateBrushModelMap["mp_nx_meteor"] = 0x83156500;
    s_CrateBrushModelMap["mp_nx_border"] = 0x83160A00;
    s_CrateBrushModelMap["mp_nx_lockdown_v2"] = 0x83155900;
    s_CrateBrushModelMap["mp_nx_contact"] = 0x83166100;
    s_CrateBrushModelMap["mp_nx_ugvcontact"] = 0x8315D100;
    s_CrateBrushModelMap["mp_nx_ugvhh"] = 0x83175400;
    s_CrateBrushModelMap["mp_nx_asylum"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    s_CrateBrushModelMap["mp_nx_asylum_2"] = 0x83155F00;
    s_CrateBrushModelMap["mp_nx_bom"] = 0x83163A00;
    s_CrateBrushModelMap["mp_nx_deadzone"] = 0x8315AA00;
    s_CrateBrushModelMap["mp_nx_dust"] = reinterpret_cast<uintptr_t>(nullptr);      // missing zone
    s_CrateBrushModelMap["mp_nx_frontline"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    s_CrateBrushModelMap["mp_nx_import"] = 0x83156500;
    s_CrateBrushModelMap["mp_nx_lunar"] = reinterpret_cast<uintptr_t>(nullptr);    // missing zone
    s_CrateBrushModelMap["mp_nx_memorial"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    s_CrateBrushModelMap["mp_nx_monorail"] = 0x8319BE00;
    s_CrateBrushModelMap["mp_nx_sandstorm"] = 0x8315FB00;
    s_CrateBrushModelMap["mp_nx_seaport"] = 0x83150B00;
    s_CrateBrushModelMap["mp_nx_skylab"] = 0x83155600;
    s_CrateBrushModelMap["mp_nx_skylight"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    s_CrateBrushModelMap["mp_nx_streets"] = 0x83159E00;
    s_CrateBrushModelMap["mp_nx_subyard"] = 0x8314E400;
    s_CrateBrushModelMap["mp_nx_ugvsand"] = 0x83179600;
    s_CrateBrushModelMap["mp_nx_whiteout"] = 0x83185F00;
    s_CrateBrushModelMap["mp_nx_leg_afghan"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    s_CrateBrushModelMap["mp_nx_leg_crash"] = 0x83161600;
    s_CrateBrushModelMap["mp_nx_leg_outpost"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    s_CrateBrushModelMap["mp_nx_leg_over"] = 0x83182900;
    s_CrateBrushModelMap["mp_nx_leg_term"] = 0x83154100;
    s_CrateBrushModelMap["mp_nx_apt"] = reinterpret_cast<uintptr_t>(nullptr);          // missing zone
    s_CrateBrushModelMap["mp_nx_binscrib"] = reinterpret_cast<uintptr_t>(nullptr);     // missing zone
    s_CrateBrushModelMap["mp_nx_g_assault"] = reinterpret_cast<uintptr_t>(nullptr);    // missing zone
    s_CrateBrushModelMap["mp_nx_merc_over"] = reinterpret_cast<uintptr_t>(nullptr);    // missing zone
    s_CrateBrushModelMap["mp_nx_mercmode"] = reinterpret_cast<uintptr_t>(nullptr);     // missing zone
    s_CrateBrushModelMap["mp_nx_test_benk_04"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    s_CrateBrushModelMap["mp_nx_blanktest"] = reinterpret_cast<uintptr_t>(nullptr);    // disc unreadable
    s_CrateBrushModelMap["mp_nx_jku"] = reinterpret_cast<uintptr_t>(nullptr);          // missing zone
    s_CrateBrushModelMap["mp_nx_jpldock"] = reinterpret_cast<uintptr_t>(nullptr);      // missing zone
    s_CrateBrushModelMap["mp_nx_milota1"] = reinterpret_cast<uintptr_t>(nullptr);      // missing zone
    s_CrateBrushModelMap["mp_nx_testmap"] = reinterpret_cast<uintptr_t>(nullptr);      // missing zone
}

gentity_s *GetCurrentMapCrateBrushModel()
{
    static bool isBrushModelMapInitialized = false;

    if (!isBrushModelMapInitialized)
    {
        InitBrushModelMap();
        isBrushModelMapInitialized = true;
    }

    std::string mapName = Dvar_GetString("ui_mapname");

    XASSERT(s_CrateBrushModelMap.find(mapName) != s_CrateBrushModelMap.end());

    return reinterpret_cast<gentity_s *>(s_CrateBrushModelMap[mapName]);
}

}
}
