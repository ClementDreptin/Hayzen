#include "pch.h"
#include "Games/AlphaMW2/GameFunctions.h"

namespace AlphaMW2
{
namespace Game
{

static std::unordered_map<std::string, uintptr_t> brushModelMap;

const char *(*SL_ConvertToString)(uint32_t stringValue) = reinterpret_cast<const char *(*)(uint32_t)>(0x8229A730);

void (*SV_GameSendServerCommand)(int clientNum, int type, const char *text) = reinterpret_cast<void (*)(int, int, const char *)>(0x822B6140);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<void (*)(int, const char *)>(0x8226F590);

const char *(*Dvar_GetString)(const char *dvarName) = reinterpret_cast<const char *(*)(const char *)>(0x82303CC0);

playerState_s *(*GetPlayerState)(int clientNum) = reinterpret_cast<playerState_s *(*)(int)>(0x8222C108);

bool (*Session_IsHost)(uintptr_t sessionDataPtr, int clientNum) = reinterpret_cast<bool (*)(uintptr_t, int)>(0x82388338);

void (*SP_script_model)(gentity_s *mSelf) = reinterpret_cast<void (*)(gentity_s *)>(0x82250A20);

gentity_s *(*G_Spawn)() = reinterpret_cast<gentity_s *(*)()>(0x82258508);

void (*G_SetModel)(gentity_s *ent, const char *modelName) = reinterpret_cast<void (*)(gentity_s *, const char *)>(0x82257B90);

void (*SV_LinkEntity)(gentity_s *gEnt) = reinterpret_cast<void (*)(gentity_s *)>(0x822C1598);

void (*SV_UnlinkEntity)(gentity_s *gEnt) = reinterpret_cast<void (*)(gentity_s *)>(0x822C14B0);

void (*SV_SetBrushModel)(gentity_s *ent) = reinterpret_cast<void (*)(gentity_s *)>(0x822B63B8);

gentity_s *(*SV_AddTestClient)() = reinterpret_cast<gentity_s *(*)()>(0x822B5F00);

void (*SV_ExecuteClientCommand)(int client, const char *s, int clientOK, int fromOldServer) = reinterpret_cast<void (*)(int, const char *, int, int)>(0x822B4700);

void (*SetClientOrigin)(gentity_s *ent, const float *origin) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x82214E70);

void (*SetClientViewAngle)(gentity_s *ent, const float *angle) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x82214C28);

void iPrintLn(int clientNum, const std::string &text)
{
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("f \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    return reinterpret_cast<gclient_s *>(0x82F01480 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(int entNum)
{
    return reinterpret_cast<gentity_s *>(0x82D47D80 + sizeof(gentity_s) * entNum);
}

void SetClientDvar(int clientNum, const std::string &dvar, const std::string &value)
{
    SV_GameSendServerCommand(clientNum, 0, Formatter::Format("v %s \"%s\"", dvar.c_str(), value.c_str()).c_str());
}

bool IsHost(int clientNum)
{
    return Session_IsHost(0x83A06F28, clientNum);
}

static void InitBrushModelMap()
{
    brushModelMap["mp_afghan"] = 0x82D60880;
    brushModelMap["mp_checkpoint"] = 0x82D76B80;
    brushModelMap["mp_derail"] = 0x82D95800;
    brushModelMap["mp_estate"] = 0x82D5B600;
    brushModelMap["mp_favela"] = 0x82D9A300;
    brushModelMap["mp_highrise"] = 0x82DC4380;
    brushModelMap["mp_invasion"] = 0x82D7E100;
    brushModelMap["mp_quarry"] = 0x82D70F00;
    brushModelMap["mp_rundown"] = 0x82D93780;
    brushModelMap["mp_rust"] = 0x82D5A980;
    brushModelMap["mp_boneyard"] = 0x82D64E80;
    brushModelMap["mp_nightshift"] = 0x82D58180;
    brushModelMap["mp_subbase"] = 0x82D7CF80;
    brushModelMap["mp_terminal"] = 0x82D61A00;
    brushModelMap["mp_underpass"] = 0x82D52000;
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
