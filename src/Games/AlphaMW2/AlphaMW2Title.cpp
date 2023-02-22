#include "pch.h"
#include "Games/AlphaMW2/AlphaMW2Title.h"

#include "Games/AlphaMW2/GameFunctions.h"

Detour *AlphaMW2Title::s_pScr_NotifyDetour = nullptr;
Detour *AlphaMW2Title::s_pSV_ExecuteClientCommandDetour = nullptr;

AlphaMW2Title::AlphaMW2Title()
    : Title()
{
    Xam::XNotify("Hayzen - MW2 Alpha Multiplayer Detected");

    // Give the system some time to fully load the game in memory
    Sleep(200);

    // Set up the function hooks
    s_pSCR_DrawScreenFieldDetour = new Detour(0x8218B5F0, SCR_DrawScreenFieldHook);
    s_pScr_NotifyDetour = new Detour(0x822539C0, Scr_NotifyHook);
    s_pSV_ExecuteClientCommandDetour = new Detour(0x822B4700, SV_ExecuteClientCommandHook);
}

AlphaMW2Title::~AlphaMW2Title()
{
    delete s_pScr_NotifyDetour;
    delete s_pSV_ExecuteClientCommandDetour;
}

void AlphaMW2Title::Scr_NotifyHook(AlphaMW2::Game::gentity_s *entity, uint16_t stringValue, uint32_t paramCount)
{
    // Call the original Scr_Notify function
    s_pScr_NotifyDetour->GetOriginal<decltype(&Scr_NotifyHook)>()(entity, stringValue, paramCount);

    // If the client is not host, no need to go further
    int clientNum = entity->state.number;
    if (!AlphaMW2::Game::IsHost(clientNum))
        return;

    // Get the string representing the event
    const char *eventName = AlphaMW2::Game::SL_ConvertToString(stringValue);

    // "begin" can happen multiple times a game in round-based gamemodes and we don't want
    // to recreate the menu every round so we make sure it's not already initialized
    if (!strcmp(eventName, "begin"))
    {
        Log::Info("game begin");

        // Disable the unlocalized error messages when printing something in the killfeed
        AlphaMW2::Game::SetClientDvar(clientNum, "loc_warnings", "0");
        AlphaMW2::Game::SetClientDvar(clientNum, "loc_warningsUI", "0");
    }
}

void AlphaMW2Title::SV_ExecuteClientCommandHook(int client, const char *s, int clientOK, int fromOldServer)
{
    // Call the original Scr_Notify SV_ExecuteClientCommand
    s_pSV_ExecuteClientCommandDetour->GetOriginal<decltype(&SV_ExecuteClientCommandHook)>()(client, s, clientOK, fromOldServer);

    // If the client is not host, no need to go further
    int clientNum = (client - Memory::Read<int>(0x83577D98)) / 0x97F80;
    if (!AlphaMW2::Game::IsHost(clientNum))
        return;

    // Stop the menu when the game ends
    if (!strcmp(s, "matchdatadone"))
        Log::Info("game end");
}
