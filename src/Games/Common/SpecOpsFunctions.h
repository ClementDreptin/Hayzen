// #pragma once is intentionally missing, this file is supposed to be included multiple times
// and generating different functions every time depending on the COMMON_FN_NAMESPACE macro.

#include "Core/Context.h"

namespace COMMON_FN_NAMESPACE
{

bool ToggleGodModeSP(bool enabled)
{
    playerState_s *pPlayerState = GetPlayerState(Context::ClientNum);
    XASSERT(pPlayerState != nullptr);

    if (enabled)
        pPlayerState->otherFlags |= 1;
    else
        pPlayerState->otherFlags &= ~1;

    return true;
}

bool ChangeJumpHeight(uint32_t value)
{
    // Set the new jump height value
    std::string command = Formatter::Format("set jump_height %d", value);
    Cbuf_AddText(0, command.c_str());

    return true;
}

bool ToggleSecondPlayerGodMode(bool enabled)
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

    playerState_s *pSecondPlayerState = GetPlayerState(secondClientNum);
    XASSERT(pSecondPlayerState != nullptr);

    if (enabled)
        pSecondPlayerState->otherFlags |= 1;
    else
        pSecondPlayerState->otherFlags &= ~1;

    return true;
}

void TeleportSecondPlayerToMe()
{
    // The second client num is always 1
    int secondClientNum = 1;
    int firstClientNum = Context::ClientNum;

    gclient_s *pSecondClient = GetGClient(secondClientNum);
    XASSERT(pSecondClient != nullptr);
    if (!pSecondClient->connected)
    {
        iPrintLn(firstClientNum, "^1No other player in the game!");
        return;
    }

    // Get the first player's current position and viewY
    playerState_s *pFirstPlayerState = GetPlayerState(firstClientNum);
    XASSERT(pFirstPlayerState != nullptr);
    vec3 origin = pFirstPlayerState->origin;
    float viewY = pFirstPlayerState->viewAngles.y;

    // Teleport the second player in front of the first player
    float distance = 100.0f;
    pSecondClient->ps.origin = Math::ProjectForward(origin, Math::Radians(viewY), distance);
}

}
