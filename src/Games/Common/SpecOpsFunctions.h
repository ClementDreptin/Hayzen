// #pragma once is intentionally missing, this file is supposed to be included multiple times
// and generating different functions every time depending on the COMMON_FN_NAMESPACE macro.

#include "Core/Menu.h"
#include "Core/Bits.h"

namespace COMMON_FN_NAMESPACE
{

// Toggle God Mode (specific to Spec Ops).
void ToggleGodModeSP(Menu *pMenu)
{
    int clientNum = pMenu->GetClientNum();

    playerState_s *pPlayerState = GetPlayerState(clientNum);

    // The default value of otherFlags is 0, the God Mode value is 1 so we just need to toggle the first bit
    // to toggle God Mode
    BIT_FLIP(pPlayerState->otherFlags, 0);

    iPrintLn(clientNum, BIT_CHECK(pPlayerState->otherFlags, 0) ? "God Mode ^2On" : "God Mode ^1Off");
}

// Threaded function that prompts a keyboard and sets the jump height value to what was entered.
static uint32_t ChangeJumpHeightThread(Menu *pMenu)
{
    // Get the value from the user via the virtual keyboard
    std::string value;
    uint32_t result = Xam::ShowKeyboard(L"Jump Height", L"Max value: 999\nDefault value: 39", L"39", value, 3, VKBD_LATIN_NUMERIC);

    // If the user canceled the keyboard, return early
    if (result != ERROR_SUCCESS)
        return 0;

    // If the user did not enter anything but still closed the keyboard by pressing START, set the value to its default value
    if (value.empty())
        value = "39";

    // Set the new jump height value
    std::string fullCommand = "set jump_height " + value;
    Cbuf_AddText(0, fullCommand.c_str());

    iPrintLn(pMenu->GetClientNum(), "Jump Height set to ^2" + value);

    return 0;
}

// Change the jump height.
void ChangeJumpHeight(Menu *pMenu)
{
    // This needs to execute on a separate thread because we need to wait for the user
    // to finish typing. If this wasn't done on a separate thread, it would block the
    // game's thread and make it crash.
    Memory::Thread(reinterpret_cast<PTHREAD_START_ROUTINE>(ChangeJumpHeightThread), pMenu);
}

// Toggle God Mode for the second player.
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

// Teleport the second player in front of the first player.
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

}
