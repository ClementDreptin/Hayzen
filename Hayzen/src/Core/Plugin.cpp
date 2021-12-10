#include "pch.h"
#include "Core\Plugin.h"

#include "Games\MW2\MW2.h"
#include "Games\MW3\MW3.h"
#include "Games\SpecOps\MW2\MW2.h"
#include "Games\SpecOps\MW3\MW3.h"
#include "Games\Alpha\MW2\MW2.h"


BOOL Plugin::s_bRunning = FALSE;
DWORD Plugin::s_dwCurrentTitle = 0;
Game* Plugin::s_CurrentGame = nullptr;


// Enum from game IDs.
enum Games
{
    GAME_DASHBOARD = 0xFFFE07D1,
    GAME_MW2 = 0x41560817,
    GAME_MW3 = 0x415608CB
};

VOID Plugin::Start()
{
    s_bRunning = TRUE;

    // Start the main loop in a separate thread.
    // We use the extended version of Thread to create a thread that won't get stopped
    // when another game is launched.
    Memory::ThreadEx((LPTHREAD_START_ROUTINE)Update, nullptr, 2);
}

VOID Plugin::Stop()
{
    s_bRunning = FALSE;

    // Wait a little bit for the system to clean things up before exiting the function
    Sleep(250);
}

DWORD Plugin::Update(LPVOID)
{
    while (s_bRunning)
    {
        // Get the current game running
        DWORD dwNewTitle = Kernel::XamGetCurrentTitleId();

        // Initialize a new game if the user launches a new game
        if (dwNewTitle != s_dwCurrentTitle)
            InitNewGame(dwNewTitle);

        // Only update the current game if it's supported
        if (s_CurrentGame)
            s_CurrentGame->Update();
    }

    return 0;
}

VOID Plugin::InitNewGame(DWORD dwNewTitle)
{
    // Clean up what previous game may have left out and reset the pointer
    delete s_CurrentGame;
    s_CurrentGame = nullptr;

    // Update the current title
    s_dwCurrentTitle = dwNewTitle;

    // Initialize the new game if it's supported
    // We have to check a string at a specific location to know if we are on the singleplayer or multiplayer XEX
    switch (dwNewTitle)
    {
    case GAME_DASHBOARD:
        Xam::XNotify("Hayzen - Dashboard Detected");
        break;
    case GAME_MW2:
        if (!strcmp((LPSTR)0x82001270, "multiplayer"))
            s_CurrentGame = new MW2();
        else if (!strcmp((LPSTR)0x8200EFE4, "startMultiplayer"))
            s_CurrentGame = new SpecOpsMW2();
        else if (!strcmp((LPSTR)0x82001D38, "multiplayer"))
            s_CurrentGame = new AlphaMW2();
        break;
    case GAME_MW3:
        if (!strcmp((LPSTR)0x82001458, "multiplayer"))
            s_CurrentGame = new MW3();
        else if (!strcmp((LPSTR)0x8200BEA8, "startMultiplayer"))
            s_CurrentGame = new SpecOpsMW3();
    default:
        break;
    }

    // Only init the new game if it's supported
    if (s_CurrentGame)
        s_CurrentGame->Init();
}

