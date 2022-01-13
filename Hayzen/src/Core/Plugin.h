#pragma once

#include "Core\Game.h"


// Main class that serves as entry point.
class Plugin
{
public:
    // Entry point that starts the main loop in a separate thread.
    static void Start();

    // Therminate the main loop.
    static void Stop();
private:
    static bool s_bRunning;
    static DWORD s_dwCurrentTitle;
    static Game *s_CurrentGame;

    // Loop as long as the plugin is running and monitor the current running game.
    static DWORD Update(void *);

    // Initialize a new game if it's supported.
    static void InitNewGame(DWORD dwNewTitle);
};
