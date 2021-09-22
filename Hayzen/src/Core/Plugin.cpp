#include "pch.h"
#include "Core\Plugin.h"

#include "Games\Games.h"


//--------------------------------------------------------------------------------------
// Static members definitions
//--------------------------------------------------------------------------------------
BOOL Plugin::s_bRunning = FALSE;
DWORD Plugin::s_dwCurrentTitle = 0;


// Enum from game IDs
enum Games
{
    GAME_DASHBOARD = 0xFFFE07D1,
    GAME_MW2 = 0x41560817,
    GAME_MW3 = 0x415608CB
};


//--------------------------------------------------------------------------------------
// Name: Start()
// Desc: Entry point that starts the main loop in a separate thread.
//--------------------------------------------------------------------------------------
VOID Plugin::Start()
{
    s_bRunning = TRUE;

    // Start the main loop in a separate thread
    Memory::Thread((LPTHREAD_START_ROUTINE)MonitorTitleId);
}


//--------------------------------------------------------------------------------------
// Name: Stop()
// Desc: Therminates the main loop.
//--------------------------------------------------------------------------------------
VOID Plugin::Stop()
{
    s_bRunning = FALSE;

    // Wait a little bit for the system to clean things up before exiting the function
    Sleep(250);
}


//--------------------------------------------------------------------------------------
// Name: MonitorTitleId()
// Desc: Loop as long as the plugin is running the monitor the current running game.
//--------------------------------------------------------------------------------------
DWORD Plugin::MonitorTitleId(LPVOID)
{
    while (s_bRunning)
    {
        // Get the current game running
        DWORD dwNewTitle = Kernel::XamGetCurrentTitleId();

        // Initialize a new game if the user launches a new game
        if (dwNewTitle != s_dwCurrentTitle)
            InitNewGame(dwNewTitle);
    }

    return 0;
}


//--------------------------------------------------------------------------------------
// Name: InitNewGame()
// Desc: Initialize a new game if it's supported.
//--------------------------------------------------------------------------------------
VOID Plugin::InitNewGame(DWORD dwNewTitle)
{
    // Cleanup what previous games may have left out
    Cleanup();

    // Update the current title
    s_dwCurrentTitle = dwNewTitle;

    // Initialize the new game if it's supported
    // We have to check a string at a specific location to know if we are on the singleplayer or multiplayer XEX
    switch (dwNewTitle)
    {
    case GAME_DASHBOARD:
        Dashboard::Init();
        break;
    case GAME_MW2:
        if (!strcmp((LPSTR)0x82001270, "multiplayer"))
            MW2::Init();
        else if (!strcmp((LPSTR)0x8200EFE4, "startMultiplayer"))
            SpecOps::MW2::Init();
        else if (!strcmp((LPSTR)0x82001D38, "multiplayer"))
            Alpha::MW2::Init();
        break;
    case GAME_MW3:
        if (!strcmp((LPSTR)0x82001458, "multiplayer"))
            MW3::Init();
        else if (!strcmp((LPSTR)0x8200BEA8, "startMultiplayer"))
            SpecOps::MW3::Init();
    default:
        return;
    }
}


//--------------------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Initialize a new game if it's supported.
//--------------------------------------------------------------------------------------
VOID Plugin::Cleanup()
{
    // Cleanup what games may have left out
    MW2::SafeReset();
    Alpha::MW2::SafeReset();
    MW3::SafeReset();
    SpecOps::MW3::SafeReset();
    SpecOps::MW2::SafeReset();
}
