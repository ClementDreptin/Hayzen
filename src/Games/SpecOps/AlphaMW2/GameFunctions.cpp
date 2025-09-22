#include "pch.h"
#include "Games/SpecOps/AlphaMW2/GameFunctions.h"

namespace SpecOpsAlphaMW2
{
namespace Game
{

void (*SV_GameSendServerCommand)(int clientNum, const char *text) = reinterpret_cast<decltype(SV_GameSendServerCommand)>(0x823329E8);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<decltype(Cbuf_AddText)>(0x82275CE8);

float (*Dvar_GetFloat)(const char *dvarName) = reinterpret_cast<decltype(Dvar_GetFloat)>(0x822D79D0);

void (*Dvar_ForEach)(void (*callback)(const dvar_t *dvar, void *data), void *userData) = reinterpret_cast<decltype(Dvar_ForEach)>(0x822DA920);

playerState_s *(*GetPlayerState)(int num) = reinterpret_cast<decltype(GetPlayerState)>(0x823329B8);

void (*SetClientOrigin)(gentity_s *ent, const float *origin) = reinterpret_cast<decltype(SetClientOrigin)>(0x821E1660);

void (*SetClientViewAngle)(gentity_s *ent, const float *angle) = reinterpret_cast<decltype(SetClientViewAngle)>(0x821E1798);

bool (*UI_AnyMenuActive)(int localClientNum) = reinterpret_cast<decltype(UI_AnyMenuActive)>(0x822BBF38);

void iPrintLn(int clientNum, const std::string &text)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, Formatter::Format("gm \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    return reinterpret_cast<gclient_s *>(0x82E4BA20 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(uint32_t entNum)
{
    return reinterpret_cast<gentity_s *>(0x82C74E28 + sizeof(gentity_s) * entNum);
}

}
}
