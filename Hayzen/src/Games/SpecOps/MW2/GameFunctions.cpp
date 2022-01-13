#include "pch.h"
#include "Games\SpecOps\MW2\GameFunctions.h"


namespace SpecOpsMW2GameFunctions
{

playerState_s *(*SV_GetPlayerstateForClientNum)(INT num) = reinterpret_cast<playerState_s *(*)(INT)>(0x8233CB30);

VOID (*TeleportPlayer)(gentity_s *player, PFLOAT origin, PFLOAT angles) = reinterpret_cast<VOID(*)(gentity_s *, PFLOAT, PFLOAT)>(0x821FF588);

gclient_s *GetGClient(INT clientNum)
{
    return reinterpret_cast<gclient_s *>(0x82EAC5E8 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(INT entNum)
{
    return reinterpret_cast<gentity_s *>(0x82CC67B8 + sizeof(gentity_s) * entNum);
}

}
