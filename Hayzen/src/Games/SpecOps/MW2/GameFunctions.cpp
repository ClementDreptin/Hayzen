#include "pch.h"
#include "Games\SpecOps\MW2\GameFunctions.h"


namespace SpecOpsMW2GameFunctions
{

VOID (*SV)(INT clientNum, LPCSTR text) = (VOID(*)(INT, LPCSTR))0x8233CB60;

VOID (*Cbuf_AddText)(INT localClientNum, LPCSTR text) = (VOID(*)(INT, LPCSTR))0x8227CF90;

playerState_s* (*SV_GetPlayerstateForClientNum)(INT num) = (playerState_s*(*)(INT))0x8233CB30;

VOID (*TeleportPlayer)(gentity_s* player, PFLOAT origin, PFLOAT angles) = (VOID(*)(gentity_s*, PFLOAT, PFLOAT))0x821FF588;

gclient_s* GetGClient(INT clientNum)
{
    return (gclient_s*)(0x82EAC5E8 + sizeof(gclient_s) * clientNum);
}

gentity_s* GetEntity(INT entNum)
{
    return (gentity_s*)(0x82CC67B8 + sizeof(gentity_s) * entNum);
}

}
