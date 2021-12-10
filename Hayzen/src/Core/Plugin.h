#pragma once

#include "Core\Game.h"


// Main class that serves as entry point.
class Plugin
{
public:
    // Entry point that starts the main loop in a separate thread.
    static VOID Start();

    // Therminate the main loop.
    static VOID Stop();
private:
    static BOOL s_bRunning;
    static DWORD s_dwCurrentTitle;
    static Game *s_CurrentGame;

    // Loop as long as the plugin is running and monitor the current running game.
    static DWORD Update(LPVOID);

    // Initialize a new game if it's supported.
    static VOID InitNewGame(DWORD dwNewTitle);
};
