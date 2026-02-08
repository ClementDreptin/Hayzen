#include "pch.h"
#include "Games/AlphaMW2/GameFunctions.h"

namespace AlphaMW2
{
namespace Game
{

static std::unordered_map<std::string, gentity_s *> s_CrateBrushModelMap;

decltype(SL_ConvertToString) SL_ConvertToString = reinterpret_cast<decltype(SL_ConvertToString)>(0x8229A730);

decltype(SV_GameSendServerCommand) SV_GameSendServerCommand = reinterpret_cast<decltype(SV_GameSendServerCommand)>(0x822B6140);

decltype(Cbuf_AddText) Cbuf_AddText = reinterpret_cast<decltype(Cbuf_AddText)>(0x8226F590);

decltype(Dvar_GetString) Dvar_GetString = reinterpret_cast<decltype(Dvar_GetString)>(0x82303CC0);

decltype(Dvar_GetFloat) Dvar_GetFloat = reinterpret_cast<decltype(Dvar_GetFloat)>(0x82303BD0);

decltype(Dvar_ForEach) Dvar_ForEach = reinterpret_cast<decltype(Dvar_ForEach)>(0x82306B80);

decltype(GetPlayerState) GetPlayerState = reinterpret_cast<decltype(GetPlayerState)>(0x8222C108);

decltype(Session_IsHost) Session_IsHost = reinterpret_cast<decltype(Session_IsHost)>(0x82388338);

decltype(SP_script_model) SP_script_model = reinterpret_cast<decltype(SP_script_model)>(0x82250A20);

decltype(G_Spawn) G_Spawn = reinterpret_cast<decltype(G_Spawn)>(0x82258508);

decltype(G_SetModel) G_SetModel = reinterpret_cast<decltype(G_SetModel)>(0x82257B90);

decltype(SV_LinkEntity) SV_LinkEntity = reinterpret_cast<decltype(SV_LinkEntity)>(0x822C1598);

decltype(SV_UnlinkEntity) SV_UnlinkEntity = reinterpret_cast<decltype(SV_UnlinkEntity)>(0x822C14B0);

decltype(SV_SetBrushModel) SV_SetBrushModel = reinterpret_cast<decltype(SV_SetBrushModel)>(0x822B63B8);

decltype(SV_AddTestClient) SV_AddTestClient = reinterpret_cast<decltype(SV_AddTestClient)>(0x822B5F00);

decltype(SV_ExecuteClientCommand) SV_ExecuteClientCommand = reinterpret_cast<decltype(SV_ExecuteClientCommand)>(0x822B4700);

decltype(SetClientOrigin) SetClientOrigin = reinterpret_cast<decltype(SetClientOrigin)>(0x82214E70);

decltype(SetClientViewAngle) SetClientViewAngle = reinterpret_cast<decltype(SetClientViewAngle)>(0x82214C28);

decltype(UI_AnyMenuActive) UI_AnyMenuActive = reinterpret_cast<decltype(UI_AnyMenuActive)>(0x822D60C0);

void iPrintLn(int clientNum, const std::string &text)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    return reinterpret_cast<gclient_s *>(0x82F01480 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(uint32_t entNum)
{
    return reinterpret_cast<gentity_s *>(0x82D47D80 + sizeof(gentity_s) * entNum);
}

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("v %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(int clientNum)
{
    if (clientNum < 0 || clientNum > 17)
        return false;

    return Session_IsHost(0x83A06F28, clientNum);
}

static void InitCrateBrushModelMap()
{
    s_CrateBrushModelMap["mp_afghan"] = GetEntity(158);
    s_CrateBrushModelMap["mp_checkpoint"] = GetEntity(300);
    s_CrateBrushModelMap["mp_derail"] = GetEntity(497);
    s_CrateBrushModelMap["mp_estate"] = GetEntity(125);
    s_CrateBrushModelMap["mp_favela"] = GetEntity(527);
    s_CrateBrushModelMap["mp_highrise"] = GetEntity(796);
    s_CrateBrushModelMap["mp_invasion"] = GetEntity(347);
    s_CrateBrushModelMap["mp_quarry"] = GetEntity(263);
    s_CrateBrushModelMap["mp_rundown"] = GetEntity(484);
    s_CrateBrushModelMap["mp_rust"] = GetEntity(120);
    s_CrateBrushModelMap["mp_boneyard"] = GetEntity(186);
    s_CrateBrushModelMap["mp_nightshift"] = GetEntity(104);
    s_CrateBrushModelMap["mp_subbase"] = GetEntity(340);
    s_CrateBrushModelMap["mp_terminal"] = GetEntity(165);
    s_CrateBrushModelMap["mp_underpass"] = GetEntity(65);
    s_CrateBrushModelMap["mp_brecourt"] = nullptr; // script compile error
}

gentity_s *GetCurrentMapCrateBrushModel()
{
    static bool isCrateBrushModelMapInitialized = false;

    if (!isCrateBrushModelMapInitialized)
    {
        InitCrateBrushModelMap();
        isCrateBrushModelMapInitialized = true;
    }

    std::string mapName = Dvar_GetString("ui_mapname");

    XASSERT(s_CrateBrushModelMap.find(mapName) != s_CrateBrushModelMap.end());

    return s_CrateBrushModelMap[mapName];
}

}
}
