#include "pch.h"
#include "Core/Plugin.h"

// #include "Games/Dashboard/Dashboard.h"
// #include "Games/MW2/MW2Title.h"
#include "Games/SpecOps/MW2/SpecOpsMW2Title.h"
#include "Games/AlphaMW2/AlphaMW2Title.h"
// #include "Games/SpecOps/AlphaMW2/SpecOpsAlphaMW2Title.h"
// #include "Games/MW3/MW3Title.h"
#include "Games/SpecOps/MW3/SpecOpsMW3Title.h"

bool Plugin::s_Running = false;
uint32_t Plugin::s_CurrentTitleId = 0;
Title *Plugin::s_pCurrentTitle = nullptr;

enum
{
    TITLE_DASHBOARD = 0xFFFE07D1,
    TITLE_MW2 = 0x41560817,
    TITLE_MW3 = 0x415608CB,
};

void Plugin::Start()
{
    s_Running = true;

    // Start the main loop in a separate thread.
    // We use the extended version of Thread to create a thread that won't get stopped
    // when another game is launched.
    Memory::ThreadEx(reinterpret_cast<PTHREAD_START_ROUTINE>(Update), nullptr, 2);
}

void Plugin::Stop()
{
    s_Running = false;

    // Wait a little bit for the system to clean things up before exiting the function
    Sleep(250);
}

uint32_t Plugin::Update(void *)
{
    while (s_Running)
    {
        // Get the current title running
        uint32_t newTitleId = Xam::GetCurrentTitleId();

        // Initialize a new title if the user launches a new title
        if (newTitleId != s_CurrentTitleId)
            InitNewTitle(newTitleId);
    }

    return 0;
}

void Plugin::InitNewTitle(uint32_t newTitleId)
{
    // Clean up what previous game may have left out and reset the pointer
    delete s_pCurrentTitle;
    s_pCurrentTitle = nullptr;

    // Update the current title
    s_CurrentTitleId = newTitleId;

    // Initialize the new game if it's supported
    // We have to check a string at a specific location to know if we are on the singleplayer or multiplayer XEX
    switch (newTitleId)
    {
    // case TITLE_DASHBOARD:
    //     Dashboard::Init();
    //     break;
    case TITLE_MW2:
        // if (!strcmp(reinterpret_cast<char *>(0x82001270), "multiplayer"))
        //     s_pCurrentTitle = new MW2Title();
        /* else */if (!strcmp(reinterpret_cast<char *>(0x8200EFE4), "startMultiplayer"))
             s_pCurrentTitle = new SpecOpsMW2Title();
        else if (!strcmp(reinterpret_cast<char *>(0x82001D38), "multiplayer"))
            s_pCurrentTitle = new AlphaMW2Title();
        // else if (!strcmp(reinterpret_cast<char *>(0x8200EDA4), "startMultiplayer"))
        //     s_pCurrentTitle = new SpecOpsAlphaMW2Title();
        break;
    case TITLE_MW3:
    //     if (!strcmp(reinterpret_cast<char *>(0x82001458), "multiplayer"))
    //         s_pCurrentTitle = new MW3Title();
        /* else */if (!strcmp(reinterpret_cast<char *>(0x8200BEA8), "startMultiplayer"))
             s_pCurrentTitle = new SpecOpsMW3Title();
        break;
    default:
        break;
    }

    // Only init the new game if it's supported
    // if (s_pCurrentTitle)
    //     s_pCurrentTitle->Init();
}
