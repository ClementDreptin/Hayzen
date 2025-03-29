// #pragma once is intentionally missing, this file is supposed to be included multiple times
// and generating different functions every time depending on the COMMON_FN_NAMESPACE macro.

#include "Core/Context.h"

namespace COMMON_FN_NAMESPACE
{

bool ToggleGodModeSP(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    playerState_s *pPlayerState = GetPlayerState(Context::ClientNum);
    XASSERT(pPlayerState != nullptr);

    if (enabled)
        pPlayerState->otherFlags |= 1;
    else
        pPlayerState->otherFlags &= ~1;

    return true;
}

bool ToggleSecondPlayerGodMode(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    // The second client num is always 1
    int secondClientNum = 1;
    int firstClientNum = Context::ClientNum;

    gclient_s *pSecondClient = GetGClient(secondClientNum);
    XASSERT(pSecondClient != nullptr);
    if (!pSecondClient->connected)
    {
        iPrintLn(firstClientNum, "^1No other player in the game!");
        return false;
    }

    playerState_s *pSecondPlayerState = GetPlayerState(secondClientNum);
    XASSERT(pSecondPlayerState != nullptr);

    if (enabled)
        pSecondPlayerState->otherFlags |= 1;
    else
        pSecondPlayerState->otherFlags &= ~1;

    return true;
}

bool TeleportSecondPlayerToMe()
{
    // The second client num is always 1
    int secondClientNum = 1;
    int firstClientNum = Context::ClientNum;

    gclient_s *pSecondClient = GetGClient(secondClientNum);
    XASSERT(pSecondClient != nullptr);
    if (!pSecondClient->connected)
    {
        iPrintLn(firstClientNum, "^1No other player in the game!");
        return false;
    }

    // Get the first player's current position and viewY
    playerState_s *pFirstPlayerState = GetPlayerState(firstClientNum);
    XASSERT(pFirstPlayerState != nullptr);
    vec3 origin = pFirstPlayerState->origin;
    float viewY = pFirstPlayerState->viewAngles.y;

    // Teleport the second player in front of the first player
    float distance = 100.0f;
    pSecondClient->ps.origin = Math::ProjectForward(origin, Math::Radians(viewY), distance);

    return true;
}

}
