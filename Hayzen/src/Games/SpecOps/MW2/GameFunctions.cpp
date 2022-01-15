#include "pch.h"
#include "Games\SpecOps\MW2\GameFunctions.h"


namespace SpecOpsMW2GameFunctions
{

void (*SV_GameSendServerCommand)(int clientNum, const char *text) = reinterpret_cast<void(*)(int, const char *)>(0x8233CB60);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<void(*)(int, const char *)>(0x8227CF90);

playerState_s *(*SV_GetPlayerstateForClientNum)(int num) = reinterpret_cast<playerState_s *(*)(int)>(0x8233CB30);

void (*TeleportPlayer)(gentity_s *player, const float *origin, const float *angles) = reinterpret_cast<void(*)(gentity_s *, const float *, const float *)>(0x821FF588);

void iPrintLn(int clientNum, const std::string &text)
{
    SV_GameSendServerCommand(clientNum, Formatter::Format("gm \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    return reinterpret_cast<gclient_s *>(0x82EAC5E8 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(int entNum)
{
    return reinterpret_cast<gentity_s *>(0x82CC67B8 + sizeof(gentity_s) * entNum);
}

}
