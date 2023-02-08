#pragma once

#include "Core/Title.h"

// Main class that serves as entry point.
class Plugin
{
public:
    // Entry point that starts the main loop in a separate thread.
    static void Start();

    // Therminate the main loop.
    static void Stop();

private:
    static bool s_Running;
    static uint32_t s_CurrentTitleId;
    static Title *s_pCurrentTitle;

    // Loop as long as the plugin is running and monitor the current running game.
    static uint32_t Update(void *);

    // Initialize a new game if it's supported.
    static void InitNewTitle(uint32_t newTitleId);
};
