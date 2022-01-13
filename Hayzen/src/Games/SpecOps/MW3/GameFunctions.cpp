#include "pch.h"
#include "Games\SpecOps\MW3\GameFunctions.h"


namespace SpecOpsMW3GameFunctions
{

playerState_s *(*SV_GetPlayerstateForClientNum)(INT num) = reinterpret_cast<playerState_s *(*)(INT)>(0x82371650);

VOID (*TeleportPlayer)(gentity_s *player, PFLOAT origin, PFLOAT angles) = reinterpret_cast<VOID(*)(gentity_s *, PFLOAT, PFLOAT)>(0x8220C960);

gclient_s *GetGClient(INT clientNum)
{
    return reinterpret_cast<gclient_s *>(0x83150090 + sizeof(gclient_s) * clientNum);
}

gentity_s *GetEntity(INT entNum)
{
    return reinterpret_cast<gentity_s *>(0x82F66560 + sizeof(gentity_s) * entNum);
}

}
