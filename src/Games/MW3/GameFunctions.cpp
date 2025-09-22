#include "pch.h"
#include "Games/MW3/GameFunctions.h"

#include "Core/Context.h"

namespace MW3
{
namespace Game
{

static std::unordered_map<std::string, uintptr_t> brushModelMap;

const char *(*SL_ConvertToString)(uint32_t stringValue) = reinterpret_cast<decltype(SL_ConvertToString)>(0x822B5120);

void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text) = reinterpret_cast<decltype(SV_GameSendServerCommand)>(0x822C9340);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<decltype(Cbuf_AddText)>(0x82287F68);

bool (*Dvar_GetBool)(const char *dvarName) = reinterpret_cast<decltype(Dvar_GetBool)>(0x8232E2C8);

const char *(*Dvar_GetString)(const char *dvarName) = reinterpret_cast<decltype(Dvar_GetString)>(0x8232E488);

playerState_s *(*GetPlayerState)(int clientNum) = reinterpret_cast<decltype(GetPlayerState)>(0x82244148);

bool (*Session_IsHost)(uintptr_t sessionDataPtr, int clientNum) = reinterpret_cast<decltype(Session_IsHost)>(0x823BFE78);

void (*SP_script_model)(gentity_s *mSelf) = reinterpret_cast<decltype(SP_script_model)>(0x82268138);

gentity_s *(*G_Spawn)() = reinterpret_cast<decltype(G_Spawn)>(0x8226F378);

void (*G_SetModel)(gentity_s *ent, const char *modelName) = reinterpret_cast<decltype(G_SetModel)>(0x8226EAA0);

void (*SV_LinkEntity)(gentity_s *gEnt) = reinterpret_cast<decltype(SV_LinkEntity)>(0x822D6E78);

void (*SV_UnlinkEntity)(gentity_s *gEnt) = reinterpret_cast<decltype(SV_UnlinkEntity)>(0x822D6D90);

bool (*SV_SetBrushModel)(gentity_s *ent) = reinterpret_cast<decltype(SV_SetBrushModel)>(0x822C95C8);

void (*SetClientOrigin)(gentity_s *ent, const float *origin) = reinterpret_cast<decltype(SetClientOrigin)>(0x8222FF50);

void (*SetClientViewAngle)(gentity_s *ent, const float *angle) = reinterpret_cast<decltype(SetClientViewAngle)>(0x8222FD10);

bool (*UI_AnyMenuActive)(int localClientNum) = reinterpret_cast<decltype(UI_AnyMenuActive)>(0x822F1680);

uint32_t (*GetProtocolVersion)() = reinterpret_cast<decltype(GetProtocolVersion)>(0x8232A320);

uint32_t (*BG_NetDataChecksum)() = reinterpret_cast<decltype(BG_NetDataChecksum)>(0x820E0B50);

int (*G_irand)(int min, int max) = reinterpret_cast<decltype(G_irand)>(0x8226FC80);

void (*SV_Cmd_TokenizeString)(const char *text_in) = reinterpret_cast<decltype(SV_Cmd_TokenizeString)>(0x822889F0);

void (*SV_Cmd_EndTokenizedString)() = reinterpret_cast<decltype(SV_Cmd_EndTokenizedString)>(0x82288A10);

void (*SV_DirectConnect)(netadr_t from) = reinterpret_cast<decltype(SV_DirectConnect)>(0x822C8AB0);

void (*SV_SendClientGameState)(client_t *client) = reinterpret_cast<decltype(SV_SendClientGameState)>(0x822C6DD0);

void (*SV_ClientEnterWorld)(client_t *client, usercmd_s *cmd) = reinterpret_cast<decltype(SV_ClientEnterWorld)>(0x822C6F50);

void (*SV_ExecuteClientCommand)(client_t *cl, const char *s, int clientOK, int fromOldServer) = reinterpret_cast<decltype(SV_ExecuteClientCommand)>(0x822C78A0);

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
