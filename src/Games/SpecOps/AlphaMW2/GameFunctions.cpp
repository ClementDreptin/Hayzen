#include "pch.h"
#include "Games/SpecOps/AlphaMW2/GameFunctions.h"

namespace SpecOpsAlphaMW2
{
namespace Game
{

void (*SV_GameSendServerCommand)(int clientNum, const char *text) = reinterpret_cast<void (*)(int, const char *)>(0x823329E8);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<void (*)(int, const char *)>(0x82275CE8);

playerState_s *(*GetPlayerState)(int num) = reinterpret_cast<playerState_s *(*)(int)>(0x823329B8);

void (*SetClientOrigin)(gentity_s *ent, const float *origin) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x821E1660);

void (*SetClientViewAngle)(gentity_s *ent, const float *angle) = reinterpret_cast<void (*)(gentity_s *, const float *)>(0x821E1798);

bool (*UI_AnyMenuActive)(int localClientNum) = reinterpret_cast<bool (*)(int)>(0x822BBF38);

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
