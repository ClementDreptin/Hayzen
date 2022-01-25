#include "pch.h"
#include "Games\SpecOps\MW3\GameFunctions.h"


namespace SpecOpsMW3GameFunctions
{

void (*SV_GameSendServerCommand)(int clientNum, const char *text) = reinterpret_cast<void(*)(int, const char *)>(0x82371680);

void (*Cbuf_AddText)(int localClientNum, const char *text) = reinterpret_cast<void(*)(int, const char *)>(0x8228E1F8);

playerState_s *(*SV_GetPlayerstateForClientNum)(int num) = reinterpret_cast<playerState_s *(*)(int)>(0x82371650);

void (*TeleportPlayer)(gentity_s *player, const float *origin, const float *angles) = reinterpret_cast<void(*)(gentity_s *, const float *, const float *)>(0x8220C960);

void iPrintLn(int clientNum, const std::string &text)
{
    SV_GameSendServerCommand(clientNum, Formatter::Format("gm \"%s\"", text.c_str()).c_str());
}

gclient_s *GetGClient(int clientNum)
{
    return reinterpret_cast<gclient_s *>(0x83150090 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(int entNum)
{
    return reinterpret_cast<gentity_s *>(0x82F66560 + sizeof(gentity_s) * entNum);
}

}
