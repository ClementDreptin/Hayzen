#include "pch.h"
#include "Games\SpecOps\MW2\GameFunctions.h"


namespace SpecOpsMW2GameFunctions
{

playerState_s *(*SV_GetPlayerstateForClientNum)(int num) = reinterpret_cast<playerState_s *(*)(int)>(0x8233CB30);

void (*TeleportPlayer)(gentity_s *player, float *origin, float *angles) = reinterpret_cast<void(*)(gentity_s *, float *, float *)>(0x821FF588);

gclient_s *GetGClient(int clientNum)
{
    return reinterpret_cast<gclient_s *>(0x82EAC5E8 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(int entNum)
{
    return reinterpret_cast<gentity_s *>(0x82CC67B8 + sizeof(gentity_s) * entNum);
}

}
