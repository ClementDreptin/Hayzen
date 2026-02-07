#include "pch.h"
#include "Games/MW2/GameFunctions.h"

namespace MW2
{
namespace Game
{

static std::unordered_map<std::string, uintptr_t> s_CrateBrushModelMap;

decltype(SL_ConvertToString) SL_ConvertToString = reinterpret_cast<decltype(SL_ConvertToString)>(0x82241898);

decltype(SV_GameSendServerCommand) SV_GameSendServerCommand = reinterpret_cast<decltype(SV_GameSendServerCommand)>(0x822548D8);

decltype(Cbuf_AddText) Cbuf_AddText = reinterpret_cast<decltype(Cbuf_AddText)>(0x82224990);

decltype(Dvar_GetBool) Dvar_GetBool = reinterpret_cast<decltype(Dvar_GetBool)>(0x8229EEE8);

decltype(Dvar_GetString) Dvar_GetString = reinterpret_cast<decltype(Dvar_GetString)>(0x8229F0A8);

decltype(GetPlayerState) GetPlayerState = reinterpret_cast<decltype(GetPlayerState)>(0x821E6628);

decltype(Session_IsHost) Session_IsHost = reinterpret_cast<decltype(Session_IsHost)>(0x82320138);

decltype(SP_script_model) SP_script_model = reinterpret_cast<decltype(SP_script_model)>(0x82206D88);

decltype(G_Spawn) G_Spawn = reinterpret_cast<decltype(G_Spawn)>(0x8220DB50);

decltype(G_SetModel) G_SetModel = reinterpret_cast<decltype(G_SetModel)>(0x8220D278);

decltype(SV_LinkEntity) SV_LinkEntity = reinterpret_cast<decltype(SV_LinkEntity)>(0x8225F518);

decltype(SV_UnlinkEntity) SV_UnlinkEntity = reinterpret_cast<decltype(SV_UnlinkEntity)>(0x8225F430);

decltype(SV_SetBrushModel) SV_SetBrushModel = reinterpret_cast<decltype(SV_SetBrushModel)>(0x82254B50);

decltype(SV_AddTestClient) SV_AddTestClient = reinterpret_cast<decltype(SV_AddTestClient)>(0x82254690);

decltype(SV_ExecuteClientCommand) SV_ExecuteClientCommand = reinterpret_cast<decltype(SV_ExecuteClientCommand)>(0x82253140);

decltype(SetClientOrigin) SetClientOrigin = reinterpret_cast<decltype(SetClientOrigin)>(0x821D3798);

decltype(SetClientViewAngle) SetClientViewAngle = reinterpret_cast<decltype(SetClientViewAngle)>(0x821D3550);

decltype(UI_AnyMenuActive) UI_AnyMenuActive = reinterpret_cast<decltype(UI_AnyMenuActive)>(0x82271E60);

void iPrintLn(int clientNum, const std::string &text)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    return reinterpret_cast<gclient_s *>(0x830CBF80 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(uint32_t entNum)
{
    return reinterpret_cast<gentity_s *>(0x82F03600 + sizeof(gentity_s) * entNum);
}

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("s %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(int clientNum)
{
    if (clientNum < 0 || clientNum > 17)
        return false;

    return Session_IsHost(0x83AC3DB0, clientNum);
}

static void InitBrushModelMap()
{
    s_CrateBrushModelMap["mp_afghan"] = 0x82F7E800;
    s_CrateBrushModelMap["mp_derail"] = 0x82F5F680;
    s_CrateBrushModelMap["mp_estate"] = 0x82F4AF00;
    s_CrateBrushModelMap["mp_favela"] = 0x82F70C00;
    s_CrateBrushModelMap["mp_highrise"] = 0x82F76380;
    s_CrateBrushModelMap["mp_invasion"] = 0x82F73180;
    s_CrateBrushModelMap["mp_checkpoint"] = 0x82F6D280;
    s_CrateBrushModelMap["mp_quarry"] = 0x82F99600;
    s_CrateBrushModelMap["mp_rundown"] = 0x82F65800;
    s_CrateBrushModelMap["mp_rust"] = 0x82F3C900;
    s_CrateBrushModelMap["mp_boneyard"] = 0x82F0B300;
    s_CrateBrushModelMap["mp_nightshift"] = 0x82F3A880;
    s_CrateBrushModelMap["mp_subbase"] = 0x82F59780;
    s_CrateBrushModelMap["mp_terminal"] = 0x82F0DD80;
    s_CrateBrushModelMap["mp_underpass"] = 0x82F66C00;
    s_CrateBrushModelMap["mp_brecourt"] = 0x82F15580;
    s_CrateBrushModelMap["mp_complex"] = 0x82F09F00;
    s_CrateBrushModelMap["mp_crash"] = 0x82F18280;
    s_CrateBrushModelMap["mp_overgrown"] = 0x82F34980;
    s_CrateBrushModelMap["mp_compact"] = 0x82F14180;
    s_CrateBrushModelMap["mp_storm"] = 0x82F5DB00;
    s_CrateBrushModelMap["mp_abandon"] = 0x82F48980;
    s_CrateBrushModelMap["mp_fuel2"] = 0x82F65300;
    s_CrateBrushModelMap["mp_strike"] = 0x82F4D980;
    s_CrateBrushModelMap["mp_trailerpark"] = 0x82F17380;
    s_CrateBrushModelMap["mp_vacant"] = 0x82F55900;
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
