#include "pch.h"
#include "Games/SpecOps/MW3/GameFunctions.h"

namespace SpecOpsMW3
{
namespace Game
{

void (*SV_GameSendServerCommand)(int clientNum, const char *text) = reinterpret_cast<decltype(SV_GameSendServerCommand)>(0x82371680);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<decltype(Cbuf_AddText)>(0x8228E1F8);

float (*Dvar_GetFloat)(const char *dvarName) = reinterpret_cast<decltype(Dvar_GetFloat)>(0x823044F0);

playerState_s *(*GetPlayerState)(int num) = reinterpret_cast<decltype(GetPlayerState)>(0x82371650);

void (*SetClientOrigin)(gentity_s *ent, const float *origin) = reinterpret_cast<decltype(SetClientOrigin)>(0x821F5018);

void (*SetClientViewAngle)(gentity_s *ent, const float *angle) = reinterpret_cast<decltype(SetClientViewAngle)>(0x821F5150);

bool (*UI_AnyMenuActive)(int localClientNum) = reinterpret_cast<decltype(UI_AnyMenuActive)>(0x822D38B8);

void iPrintLn(int clientNum, const std::string &text)
{
    XASSERT(clientNum >= -1 && clientNum <= 17);

    SV_GameSendServerCommand(clientNum, Formatter::Format("gm \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    XASSERT(clientNum >= 0 && clientNum <= 17);

    return reinterpret_cast<gclient_s *>(0x83150090 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(uint32_t entNum)
{
    return reinterpret_cast<gentity_s *>(0x82F66560 + sizeof(gentity_s) * entNum);
}

}
}
