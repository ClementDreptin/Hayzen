#include "pch.h"
#include "Games/AlphaGhosts/GameFunctions.h"

namespace AlphaGhosts
{
namespace Game
{

static std::unordered_map<std::string, uintptr_t> brushModelMap;

decltype(SL_ConvertToString) SL_ConvertToString = reinterpret_cast<decltype(SL_ConvertToString)>(0x826F90B8);

decltype(SV_GameSendServerCommand) SV_GameSendServerCommand = reinterpret_cast<decltype(SV_GameSendServerCommand)>(0x82769058);

decltype(Cbuf_AddText) Cbuf_AddText = reinterpret_cast<decltype(Cbuf_AddText)>(0x8266A130);

decltype(Dvar_GetFloat) Dvar_GetFloat = reinterpret_cast<decltype(Dvar_GetFloat)>(0x82832C58);

decltype(Dvar_GetString) Dvar_GetString = reinterpret_cast<decltype(Dvar_GetString)>(0x82832D20);

decltype(GetPlayerState) GetPlayerState = reinterpret_cast<decltype(GetPlayerState)>(0x825FE810);

decltype(Session_IsHost) Session_IsHost = reinterpret_cast<decltype(Session_IsHost)>(0x82914CE8);

decltype(SP_script_model) SP_script_model = reinterpret_cast<decltype(SP_script_model)>(0x82638078);

decltype(G_Spawn) G_Spawn = reinterpret_cast<decltype(G_Spawn)>(0x826410A0);

decltype(G_SetModel) G_SetModel = reinterpret_cast<decltype(G_SetModel)>(0x82642428);

decltype(SV_LinkEntity) SV_LinkEntity = reinterpret_cast<decltype(SV_LinkEntity)>(0x8278BB40);

decltype(SV_UnlinkEntity) SV_UnlinkEntity = reinterpret_cast<decltype(SV_UnlinkEntity)>(0x8278B6D0);

decltype(SV_SetBrushModel) SV_SetBrushModel = reinterpret_cast<decltype(SV_SetBrushModel)>(0x82769280);

decltype(SetClientOrigin) SetClientOrigin = reinterpret_cast<decltype(SetClientOrigin)>(0x825DD288);

decltype(SetClientViewAngle) SetClientViewAngle = reinterpret_cast<decltype(SetClientViewAngle)>(0x825DCFF8);

decltype(UI_AnyMenuActive) UI_AnyMenuActive = reinterpret_cast<decltype(UI_AnyMenuActive)>(0x827CD838);

void iPrintLn(int clientNum, const std::string &text)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    // This is what iPrintLnBold does technically but iPrintLn doesn't work, I don't know why...
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("g \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    return reinterpret_cast<gclient_s *>(0x8385E700 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(uint32_t entNum)
{
    return reinterpret_cast<gentity_s *>(0x8390E000 + sizeof(gentity_s) * entNum);
}

bool IsHost(int clientNum)
{
    if (clientNum < 0 || clientNum > 17)
        return false;

    return Session_IsHost(0x84A8B458, clientNum);
}

static void InitBrushModelMap()
{
    // The maps set to nullptr don't work and there's a comment with the error

    brushModelMap["mp_dome"] = 0x8391CB00;
    brushModelMap["mp_plaza2"] = 0x83918000;
    brushModelMap["mp_paris"] = reinterpret_cast<uintptr_t>(nullptr); // missing zone
    brushModelMap["mp_lonestar"] = 0x83927280;
    brushModelMap["mp_frag"] = 0x8391A800;
    brushModelMap["mp_snow"] = 0x83919180;
    brushModelMap["mp_fahrenheit"] = 0x83924800;
    brushModelMap["mp_hasima"] = reinterpret_cast<uintptr_t>(nullptr); // disc unreadable
    brushModelMap["mp_warhawk"] = 0x83917D80;
    brushModelMap["mp_sovereign"] = 0x83919400;
    brushModelMap["mp_zebra"] = 0x83918780;
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
        pBrushModel = reinterpret_cast<gentity_s *>(0x83917B00);

    return pBrushModel;
}

}
}
