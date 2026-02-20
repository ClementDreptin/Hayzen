#include "pch.h"
#include "Games/WaW/GameFunctions.h"

namespace WaW
{
namespace Game
{

static std::unordered_map<std::string, gentity_s *> s_CrateBrushModelMap;

decltype(SL_ConvertToString) SL_ConvertToString = reinterpret_cast<decltype(SL_ConvertToString)>(0x8233AD40);

decltype(SV_GameSendServerCommand) SV_GameSendServerCommand = reinterpret_cast<decltype(SV_GameSendServerCommand)>(0x82285FA8);

decltype(Cbuf_AddText) Cbuf_AddText = reinterpret_cast<decltype(Cbuf_AddText)>(0x8226FF08);

decltype(Dvar_GetBool) Dvar_GetBool = reinterpret_cast<decltype(Dvar_GetBool)>(0x822BE0E8);

decltype(Dvar_GetString) Dvar_GetString = reinterpret_cast<decltype(Dvar_GetString)>(0x822BE230);

decltype(Dvar_GetFloat) Dvar_GetFloat = reinterpret_cast<decltype(Dvar_GetFloat)>(0x822BE1C0);

decltype(SP_script_model) SP_script_model = reinterpret_cast<decltype(SP_script_model)>(0x82243760);

decltype(G_Spawn) G_Spawn = reinterpret_cast<decltype(G_Spawn)>(0x8225D510);

decltype(G_SetModel) G_SetModel = reinterpret_cast<decltype(G_SetModel)>(0x8225CDE0);

decltype(SV_LinkEntity) SV_LinkEntity = reinterpret_cast<decltype(SV_LinkEntity)>(0x82290C38);

decltype(SV_UnlinkEntity) SV_UnlinkEntity = reinterpret_cast<decltype(SV_UnlinkEntity)>(0x82290BF0);

decltype(SV_SetBrushModel) SV_SetBrushModel = reinterpret_cast<decltype(SV_SetBrushModel)>(0x82286190);

decltype(SV_AddTestClient) SV_AddTestClient = reinterpret_cast<decltype(SV_AddTestClient)>(0x82285D20);

decltype(SV_ExecuteClientCommand) SV_ExecuteClientCommand = reinterpret_cast<decltype(SV_ExecuteClientCommand)>(0x82284A78);

decltype(SetClientViewAngle) SetClientViewAngle = reinterpret_cast<decltype(SetClientViewAngle)>(0x8220A350);

decltype(UI_AnyMenuActive) UI_AnyMenuActive = reinterpret_cast<decltype(UI_AnyMenuActive)>(0x8229F100);

decltype(BG_PlayerStateToEntityState) BG_PlayerStateToEntityState = reinterpret_cast<decltype(BG_PlayerStateToEntityState)>(0x82135A00);

void iPrintLn(int clientNum, const std::string &text)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    return reinterpret_cast<gclient_s *>(0x82D0FA18 + sizeof(gclient_s) * clientNum);
}

playerState_s *GetPlayerState(int clientNum)
{
    // This function is inlined in WaW so it had to be reimplemented

    return &GetGClient(clientNum)->ps;
}

gentity_s *GetEntity(uint32_t entNum)
{
    return reinterpret_cast<gentity_s *>(0x82BAD1B0 + sizeof(gentity_s) * entNum);
}

void SetClientOrigin(gentity_s *ent, const float *origin)
{
    // This function is inlined in WaW so it had to be reimplemented

    ent->client->ps.origin.x = origin[0];
    ent->client->ps.origin.y = origin[1];
    ent->client->ps.origin.z = origin[2] + 1.0f;
    ent->client->ps.eFlags ^= 2;
    BG_PlayerStateToEntityState(&ent->client->ps, &ent->state, 1, 1);
    ent->r.currentOrigin = ent->client->ps.origin;
}

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("v %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(int clientNum)
{
    // I'm pretty sure the host is always client 0 on WaW
    // TODO: check if this is true
    return clientNum == 0;
}

static void InitCrateBrushModelMap()
{
    s_CrateBrushModelMap["mp_airfield"] = GetEntity(240);
    s_CrateBrushModelMap["mp_airfield"]->r.currentAngles = vec3(0.0f, 0.0f, 15.0f);

    s_CrateBrushModelMap["mp_asylum"] = GetEntity(105);
    s_CrateBrushModelMap["mp_asylum"]->r.currentAngles = vec3(0.0f, 90.0f, 15.0f);

    s_CrateBrushModelMap["mp_castle"] = GetEntity(542);
    s_CrateBrushModelMap["mp_castle"]->r.currentAngles = vec3(0.0f, 90.0f, 15.0f);

    s_CrateBrushModelMap["mp_shrine"] = GetEntity(270);
    s_CrateBrushModelMap["mp_shrine"]->r.currentAngles = vec3(0.0f, 340.6f, 15.0f);

    s_CrateBrushModelMap["mp_courtyard"] = GetEntity(130);
    s_CrateBrushModelMap["mp_courtyard"]->r.currentAngles = vec3(0.0f, 90.0f, 15.0f);

    s_CrateBrushModelMap["mp_dome"] = GetEntity(65);
    s_CrateBrushModelMap["mp_dome"]->r.currentAngles = vec3(0.0f, 90.0f, 15.0f);

    s_CrateBrushModelMap["mp_downfall"] = GetEntity(115);
    s_CrateBrushModelMap["mp_downfall"]->r.currentAngles = vec3(0.0f, 180.0f, 15.0f);

    s_CrateBrushModelMap["mp_hangar"] = GetEntity(51);
    s_CrateBrushModelMap["mp_hangar"]->r.currentAngles = vec3(0.0f, 180.0f, 15.0f);

    s_CrateBrushModelMap["mp_makin"] = GetEntity(41);
    s_CrateBrushModelMap["mp_makin"]->r.currentAngles = vec3(0.0f, 0.0f, 15.0f);

    s_CrateBrushModelMap["mp_outskirts"] = GetEntity(61);
    s_CrateBrushModelMap["mp_outskirts"]->r.currentAngles = vec3(0.0f, 270.0f, 15.0f);

    s_CrateBrushModelMap["mp_roundhouse"] = GetEntity(60);
    s_CrateBrushModelMap["mp_roundhouse"]->r.currentAngles = vec3(0.0f, 180.0f, 15.0f);

    s_CrateBrushModelMap["mp_seelow"] = GetEntity(132);
    s_CrateBrushModelMap["mp_seelow"]->r.currentAngles = vec3(0.0f, 90.0f, 15.0f);

    s_CrateBrushModelMap["mp_suburban"] = GetEntity(355);
    s_CrateBrushModelMap["mp_suburban"]->r.currentAngles = vec3(0.0f, 180.0f, 15.0f);

    s_CrateBrushModelMap["mp_makin_day"] = GetEntity(41);
    s_CrateBrushModelMap["mp_makin_day"]->r.currentAngles = vec3(0.0f, 0.0f, 15.0f);

    s_CrateBrushModelMap["mp_subway"] = GetEntity(136);
    s_CrateBrushModelMap["mp_subway"]->r.currentAngles = vec3(0.0f, 343.8f, 15.0f);

    s_CrateBrushModelMap["mp_kneedeep"] = GetEntity(45);
    s_CrateBrushModelMap["mp_kneedeep"]->r.currentAngles = vec3(0.0f, 24.1f, 15.0f);

    s_CrateBrushModelMap["mp_nachtfeuer"] = GetEntity(56);
    s_CrateBrushModelMap["mp_nachtfeuer"]->r.currentAngles = vec3(0.0f, 180.0f, 15.0f);

    s_CrateBrushModelMap["mp_docks"] = GetEntity(61);
    s_CrateBrushModelMap["mp_docks"]->r.currentAngles = vec3(0.0f, 0.0f, 15.0f);

    s_CrateBrushModelMap["mp_stalingrad"] = GetEntity(70);
    s_CrateBrushModelMap["mp_stalingrad"]->r.currentAngles = vec3(0.0f, 90.0f, 15.0f);

    s_CrateBrushModelMap["mp_kwai"] = GetEntity(41);
    s_CrateBrushModelMap["mp_kwai"]->r.currentAngles = vec3(0.0f, 95.0f, 15.0f);

    s_CrateBrushModelMap["mp_bgate"] = GetEntity(51);
    s_CrateBrushModelMap["mp_bgate"]->r.currentAngles = vec3(0.0f, 49.2f, 15.0f);

    s_CrateBrushModelMap["mp_vodka"] = GetEntity(437);
    s_CrateBrushModelMap["mp_vodka"]->r.currentAngles = vec3(0.0f, 0.0f, 15.0f);

    s_CrateBrushModelMap["mp_drum"] = GetEntity(41);
    s_CrateBrushModelMap["mp_drum"]->r.currentAngles = vec3(0.0f, 90.0f, 15.0f);
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
