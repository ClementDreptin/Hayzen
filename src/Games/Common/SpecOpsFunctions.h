// #pragma once is intentionally missing, this file is supposed to be included multiple times
// and generating different functions every time depending on the COMMON_FN_NAMESPACE macro.

#include "Core/Context.h"

namespace COMMON_FN_NAMESPACE
{

void ToggleGodModeSP(void *pParameters)
{
    bool enabled = *reinterpret_cast<bool *>(pParameters);

    playerState_s *pPlayerState = GetPlayerState(Context::ClientNum);

    pPlayerState->otherFlags = enabled ? 1 : 0;

}

void ChangeJumpHeight(void *pParameters)
{
    uint32_t value = *reinterpret_cast<uint32_t *>(pParameters);

    // Set the new jump height value
    std::string command = "set jump_height " + std::to_string(static_cast<uint64_t>(value));
    Cbuf_AddText(0, command.c_str());
}
/*
void ToggleSecondPlayerGodMode(Menu *pMenu)
{
    // The second client num is always 1
    int secondClientNum = 1;
    int firstClientNum = pMenu->GetClientNum();

    // If the player name of the second client is empty, it means there is no second client
    gclient_s *pSecondClient = GetGClient(secondClientNum);
    if (!pSecondClient->connected)
    {
        iPrintLn(firstClientNum, "^1No other player in the game!");
        return;
    }

    playerState_s *pSecondPlayerState = GetPlayerState(secondClientNum);

    BIT_FLIP(pSecondPlayerState->otherFlags, 0);

    iPrintLn(firstClientNum, BIT_CHECK(pSecondPlayerState->otherFlags, 0) ? "Second Player God Mode ^2On" : "Second Player God Mode ^1Off");
}

void TeleportSecondPlayerToMe(Menu *pMenu)
{
    // The second client num is always 1
    int secondClientNum = 1;
    int firstClientNum = pMenu->GetClientNum();

    gclient_s *pSecondClient = GetGClient(secondClientNum);
    if (!pSecondClient->connected)
    {
        iPrintLn(firstClientNum, "^1No other player in the game!");
        return;
    }

    // Get the first player's current position
    float distance = 100.0f;
    vec3 origin = GetPlayerState(firstClientNum)->origin;
    float viewY = GetPlayerState(firstClientNum)->viewAngles.y;

    // Teleport the second player in front of the first player
    pSecondClient->ps.origin = Math::ToFront(origin, viewY, distance);
}
*/
}
