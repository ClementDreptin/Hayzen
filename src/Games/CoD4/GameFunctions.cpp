#include "pch.h"
#include "Games/CoD4/GameFunctions.h"

namespace CoD4
{
namespace Game
{

static std::unordered_map<std::string, gentity_s *> s_CrateBrushModelMap;

decltype(SV_GameSendServerCommand) SV_GameSendServerCommand = reinterpret_cast<decltype(SV_GameSendServerCommand)>(0x82204BB8);

decltype(Cbuf_AddText) Cbuf_AddText = reinterpret_cast<decltype(Cbuf_AddText)>(0x82239FD0);

decltype(Dvar_GetBool) Dvar_GetBool = reinterpret_cast<decltype(Dvar_GetBool)>(0x821D15D8);

decltype(Dvar_GetString) Dvar_GetString = reinterpret_cast<decltype(Dvar_GetString)>(0x821D1478);

decltype(Dvar_GetFloat) Dvar_GetFloat = reinterpret_cast<decltype(Dvar_GetFloat)>(0x821D14F8);

decltype(SP_script_model) SP_script_model = reinterpret_cast<decltype(SP_script_model)>(0x82256B20);

decltype(G_Spawn) G_Spawn = reinterpret_cast<decltype(G_Spawn)>(0x8224C898);

decltype(G_SetModel) G_SetModel = reinterpret_cast<decltype(G_SetModel)>(0x8224D030);

decltype(SV_LinkEntity) SV_LinkEntity = reinterpret_cast<decltype(SV_LinkEntity)>(0x82355A00);

decltype(SV_UnlinkEntity) SV_UnlinkEntity = reinterpret_cast<decltype(SV_UnlinkEntity)>(0x82355F08);

decltype(SV_SetBrushModel) SV_SetBrushModel = reinterpret_cast<decltype(SV_SetBrushModel)>(0x82205050);

decltype(SV_AddTestClient) SV_AddTestClient = reinterpret_cast<decltype(SV_AddTestClient)>(0x82207DD8);

decltype(SV_ExecuteClientCommand) SV_ExecuteClientCommand = reinterpret_cast<decltype(SV_ExecuteClientCommand)>(0x82208088);

decltype(SetClientViewAngle) SetClientViewAngle = reinterpret_cast<decltype(SetClientViewAngle)>(0x82284C60);

decltype(UI_AnyMenuActive) UI_AnyMenuActive = reinterpret_cast<decltype(UI_AnyMenuActive)>(0x821EA338);

decltype(DB_FindXAssetHeader) DB_FindXAssetHeader = reinterpret_cast<decltype(DB_FindXAssetHeader)>(0x822A0298);

decltype(BG_PlayerStateToEntityState) BG_PlayerStateToEntityState = reinterpret_cast<decltype(BG_PlayerStateToEntityState)>(0x8233CBE8);

int R_TextHeight(UI::Font_s *pFont)
{
    // This function is inlined in CoD4 so it had to be reimplemented

    return pFont->pixelHeight;
}

UI::Font_s *R_RegisterFont(const char *font, int imageTrack)
{
    // This function is inlined in CoD4 so it had to be reimplemented

    return static_cast<UI::Font_s *>(DB_FindXAssetHeader(0x14, font));
}

HANDLE Material_RegisterHandle(const char *name, int imageTrack)
{
    // This function is inlined in CoD4 so it had to be reimplemented

    return static_cast<HANDLE>(DB_FindXAssetHeader(0x04, name));
}

const char *SL_ConvertToString(uint32_t stringValue)
{
    // This function is inlined in CoD4 so it had to be reimplemented

    if (stringValue == 0)
        return nullptr;

    const char *buffer = Memory::Read<const char *>(0x82B8576C);

    return &buffer[stringValue * 12 + 4];
}

void iPrintLn(int clientNum, const std::string &text)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    return reinterpret_cast<gclient_s *>(0x829BCD70 + sizeof(gclient_s) * clientNum);
}

playerState_s *GetPlayerState(int clientNum)
{
    // This function is inlined in CoD4 so it had to be reimplemented

    return &GetGClient(clientNum)->ps;
}

gentity_s *GetEntity(uint32_t entNum)
{
    return reinterpret_cast<gentity_s *>(0x8287CD08 + sizeof(gentity_s) * entNum);
}

void SetClientOrigin(gentity_s *ent, const float *origin)
{
    // This function is inlined in CoD4 so it had to be reimplemented

    ent->client->ps.origin.x = origin[0];
    ent->client->ps.origin.y = origin[1];
    ent->client->ps.origin.z = origin[2] + 1.0f;
    ent->client->ps.eFlags ^= 2;
    BG_PlayerStateToEntityState(&ent->client->ps, &ent->state, 1, 1);
    ent->r.currentOrigin = ent->client->ps.origin;
}

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("v %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(int clientNum)
{
    // I'm pretty sure the host is always client 0 on CoD4
    return clientNum == 0;
}

static void InitBrushModelMap()
{
    s_CrateBrushModelMap["mp_convoy"] = GetEntity(44);
    s_CrateBrushModelMap["mp_convoy"]->r.currentAngles = vec3(360.0f, 50.0f, 10.0f);

    s_CrateBrushModelMap["mp_backlot"] = GetEntity(37);
    s_CrateBrushModelMap["mp_backlot"]->r.currentAngles = vec3(0.0f, 90.0f, 0.0f);

    s_CrateBrushModelMap["mp_bloc"] = GetEntity(375);
    s_CrateBrushModelMap["mp_bloc"]->r.currentAngles = vec3(0.0f, 0.0f, 0.0f);

    s_CrateBrushModelMap["mp_bog"] = GetEntity(292);
    s_CrateBrushModelMap["mp_bog"]->r.currentAngles = vec3(3.2984f, 225.108f, 1.16511f);

    s_CrateBrushModelMap["mp_countdown"] = GetEntity(291);
    s_CrateBrushModelMap["mp_countdown"]->r.currentAngles = vec3(0.0f, 0.0f, 0.0f);

    s_CrateBrushModelMap["mp_crash"] = GetEntity(287);
    s_CrateBrushModelMap["mp_crash"]->r.currentAngles = vec3(0.0f, 90.0f, 0.0f);

    s_CrateBrushModelMap["mp_crossfire"] = GetEntity(369);
    s_CrateBrushModelMap["mp_crossfire"]->r.currentAngles = vec3(0.0f, 325.0f, 0.0f);

    s_CrateBrushModelMap["mp_citystreets"] = GetEntity(69);
    s_CrateBrushModelMap["mp_citystreets"]->r.currentAngles = vec3(0.0f, 268.0f, 0.0f);

    s_CrateBrushModelMap["mp_farm"] = GetEntity(385);
    s_CrateBrushModelMap["mp_farm"]->r.currentAngles = vec3(3.84859f, 0.0140285f, 0.208819f);

    s_CrateBrushModelMap["mp_overgrown"] = GetEntity(35);
    s_CrateBrushModelMap["mp_overgrown"]->r.currentAngles = vec3(1.0f, 95.9f, 0.0f);

    s_CrateBrushModelMap["mp_pipeline"] = GetEntity(45);
    s_CrateBrushModelMap["mp_pipeline"]->r.currentAngles = vec3(0.0f, 180.0f, 0.0f);

    s_CrateBrushModelMap["mp_shipment"] = GetEntity(209);
    s_CrateBrushModelMap["mp_shipment"]->r.currentAngles = vec3(0.0f, 270.0f, 0.0f);

    s_CrateBrushModelMap["mp_showdown"] = GetEntity(287);
    s_CrateBrushModelMap["mp_showdown"]->r.currentAngles = vec3(0.0f, 0.0f, 0.0f);

    s_CrateBrushModelMap["mp_strike"] = GetEntity(245);
    s_CrateBrushModelMap["mp_strike"]->r.currentAngles = vec3(0.0f, 180.0f, 0.0f);

    s_CrateBrushModelMap["mp_vacant"] = GetEntity(364);
    s_CrateBrushModelMap["mp_vacant"]->r.currentAngles = vec3(0.0f, 90.0f, 0.0f);

    s_CrateBrushModelMap["mp_cargoship"] = GetEntity(460);
    s_CrateBrushModelMap["mp_cargoship"]->r.currentAngles = vec3(0.0f, 90.0f, 0.0f);

    s_CrateBrushModelMap["mp_broadcast"] = GetEntity(35);
    s_CrateBrushModelMap["mp_broadcast"]->r.currentAngles = vec3(0.0f, 359.0f, 0.0f);

    s_CrateBrushModelMap["mp_carentan"] = GetEntity(233);
    s_CrateBrushModelMap["mp_carentan"]->r.currentAngles = vec3(0.0f, 0.0f, 0.0f);

    s_CrateBrushModelMap["mp_killhouse"] = GetEntity(67);
    s_CrateBrushModelMap["mp_killhouse"]->r.currentAngles = vec3(0.0f, 0.0f, 0.0f);

    s_CrateBrushModelMap["mp_creek"] = GetEntity(36);
    s_CrateBrushModelMap["mp_creek"]->r.currentAngles = vec3(0.0f, 250.1f, 0.0f);
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

    return s_CrateBrushModelMap[mapName];
}

}
}
