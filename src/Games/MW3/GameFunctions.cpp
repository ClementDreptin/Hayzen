#include "pch.h"
#include "Games/MW3/GameFunctions.h"

#include "Core/Context.h"

namespace MW3
{
namespace Game
{

static std::unordered_map<std::string, uintptr_t> s_CrateBrushModelMap;

decltype(SL_ConvertToString) SL_ConvertToString = reinterpret_cast<decltype(SL_ConvertToString)>(0x822B5120);

decltype(SV_GameSendServerCommand) SV_GameSendServerCommand = reinterpret_cast<decltype(SV_GameSendServerCommand)>(0x822C9340);

decltype(Cbuf_AddText) Cbuf_AddText = reinterpret_cast<decltype(Cbuf_AddText)>(0x82287F68);

decltype(Dvar_GetBool) Dvar_GetBool = reinterpret_cast<decltype(Dvar_GetBool)>(0x8232E2C8);

decltype(Dvar_GetString) Dvar_GetString = reinterpret_cast<decltype(Dvar_GetString)>(0x8232E488);

decltype(GetPlayerState) GetPlayerState = reinterpret_cast<decltype(GetPlayerState)>(0x82244148);

decltype(Session_IsHost) Session_IsHost = reinterpret_cast<decltype(Session_IsHost)>(0x823BFE78);

decltype(SP_script_model) SP_script_model = reinterpret_cast<decltype(SP_script_model)>(0x82268138);

decltype(G_Spawn) G_Spawn = reinterpret_cast<decltype(G_Spawn)>(0x8226F378);

decltype(G_SetModel) G_SetModel = reinterpret_cast<decltype(G_SetModel)>(0x8226EAA0);

decltype(SV_LinkEntity) SV_LinkEntity = reinterpret_cast<decltype(SV_LinkEntity)>(0x822D6E78);

decltype(SV_UnlinkEntity) SV_UnlinkEntity = reinterpret_cast<decltype(SV_UnlinkEntity)>(0x822D6D90);

decltype(SV_SetBrushModel) SV_SetBrushModel = reinterpret_cast<decltype(SV_SetBrushModel)>(0x822C95C8);

decltype(SetClientOrigin) SetClientOrigin = reinterpret_cast<decltype(SetClientOrigin)>(0x8222FF50);

decltype(SetClientViewAngle) SetClientViewAngle = reinterpret_cast<decltype(SetClientViewAngle)>(0x8222FD10);

decltype(UI_AnyMenuActive) UI_AnyMenuActive = reinterpret_cast<decltype(UI_AnyMenuActive)>(0x822F1680);

decltype(GetProtocolVersion) GetProtocolVersion = reinterpret_cast<decltype(GetProtocolVersion)>(0x8232A320);

decltype(BG_NetDataChecksum) BG_NetDataChecksum = reinterpret_cast<decltype(BG_NetDataChecksum)>(0x820E0B50);

decltype(G_irand) G_irand = reinterpret_cast<decltype(G_irand)>(0x8226FC80);

decltype(SV_Cmd_TokenizeString) SV_Cmd_TokenizeString = reinterpret_cast<decltype(SV_Cmd_TokenizeString)>(0x822889F0);

decltype(SV_Cmd_EndTokenizedString) SV_Cmd_EndTokenizedString = reinterpret_cast<decltype(SV_Cmd_EndTokenizedString)>(0x82288A10);

decltype(SV_DirectConnect) SV_DirectConnect = reinterpret_cast<decltype(SV_DirectConnect)>(0x822C8AB0);

decltype(SV_SendClientGameState) SV_SendClientGameState = reinterpret_cast<decltype(SV_SendClientGameState)>(0x822C6DD0);

decltype(SV_ClientEnterWorld) SV_ClientEnterWorld = reinterpret_cast<decltype(SV_ClientEnterWorld)>(0x822C6F50);

decltype(SV_ExecuteClientCommand) SV_ExecuteClientCommand = reinterpret_cast<decltype(SV_ExecuteClientCommand)>(0x822C78A0);

// SV_AddTestClient is empty on MW3 so it needs to be reimplemented from scratch
gentity_s *SV_AddTestClient()
{
    // Credits go to https://github.com/aaa0x for this implementation

    const size_t MAX_CLIENTS = 18;

    // Get a pointer to the array of client_t's
    client_t *svsClients = Memory::Read<client_t *>(0x834C0480 + 0x205E90); // svs + offsetof(serverStatic_t, clients) = 0x836C6310
    XASSERT(svsClients != nullptr);

    // Look for an available slot for the bot
    size_t firstAvailableSlot;
    for (firstAvailableSlot = 0; firstAvailableSlot < MAX_CLIENTS; firstAvailableSlot++)
    {
        client_t *pClient = &svsClients[firstAvailableSlot];
        if (pClient->header.state == CS_FREE)
            break;
    }

    // Return early if the match is full
    if (firstAvailableSlot == MAX_CLIENTS)
    {
        iPrintLn(Context::ClientNum, "Match is full!");
        return nullptr;
    }

    // Create the bot info
    char userInfo[1024] = {};
    _snprintf_s(
        userInfo,
        _TRUNCATE,
        "connect bot%d \"snaps\\20\\rate\\5000\\name\\bot%d\\natType\\1\\protocol\\%i\\checksum\\%i\\"
        "challenge\\0\\statver\\26 3648679816\\invited\\1\\xuid\\%08x%08x\\onlineStats\\0\\migrating\\0\"",
        firstAvailableSlot - 1,
        firstAvailableSlot,
        GetProtocolVersion(),
        BG_NetDataChecksum(),
        G_irand(0, INT32_MAX),
        G_irand(0, INT32_MAX)
    );

    // Create the bot address
    netadr_t botAddress = {};
    botAddress.type = NA_BOT;

    // Make the bot connect to the match
    SV_Cmd_TokenizeString(userInfo);
    SV_DirectConnect(botAddress);
    SV_Cmd_EndTokenizedString();

    // Set basic client info
    client_t *pClient = &svsClients[firstAvailableSlot];
    pClient->scriptId = 1023;
    pClient->bIsTestClient = 1;
    pClient->gentity->state.number = firstAvailableSlot;

    // Make the bot enter the world
    usercmd_s cmd = {};
    SV_SendClientGameState(pClient);
    SV_ClientEnterWorld(pClient, &cmd);

    return pClient->gentity;
}

void iPrintLn(int clientNum, const std::string &text)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    return reinterpret_cast<gclient_s *>(0x82F99580 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(uint32_t entNum)
{
    return reinterpret_cast<gentity_s *>(0x82DCCC80 + sizeof(gentity_s) * entNum);
}

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("q %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(int clientNum)
{
    if (clientNum < 0 || clientNum > 17)
        return false;

    return Session_IsHost(0x83BC0148, clientNum);
}

static void InitBrushModelMap()
{
    s_CrateBrushModelMap["mp_seatown"] = 0x82DD1280;
    s_CrateBrushModelMap["mp_mogadishu"] = 0x82E08A00;
    s_CrateBrushModelMap["mp_exchange"] = 0x82E14580;
    s_CrateBrushModelMap["mp_radar"] = 0x82DD3A80;
    s_CrateBrushModelMap["mp_terminal_cls"] = 0x82DF9C80;
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

    gentity_s *pBrushModel = reinterpret_cast<gentity_s *>(s_CrateBrushModelMap[mapName]);
    if (!pBrushModel)
        pBrushModel = reinterpret_cast<gentity_s *>(0x82DD1500);

    return pBrushModel;
}

}
}
