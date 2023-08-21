#include "pch.h"
#include "Core/Plugin.h"

#include "Games/Dashboard/Dashboard.h"
#include "Games/MW2/MW2Title.h"
#include "Games/SpecOps/MW2/SpecOpsMW2Title.h"
#include "Games/AlphaMW2/AlphaMW2Title.h"
#include "Games/SpecOps/AlphaMW2/SpecOpsAlphaMW2Title.h"
#include "Games/MW3/MW3Title.h"
#include "Games/SpecOps/MW3/SpecOpsMW3Title.h"

enum
{
    TITLE_DASHBOARD = 0xFFFE07D1,
    TITLE_MW2 = 0x41560817,
    TITLE_MW3 = 0x415608CB,
};

Plugin::Plugin()
    : m_Running(true)
{
    // Start the main loop in a separate thread.
    // We use the extended version of Thread to create a thread that won't get stopped
    // when another game is launched.
    Memory::ThreadEx(reinterpret_cast<PTHREAD_START_ROUTINE>(UpdateThread), this, EXCREATETHREAD_SYSTEM);
}

Plugin::~Plugin()
{
    m_Running = false;

    // Wait a little bit for the system to clean things up before exiting the function
    Sleep(250);
}

std::string Plugin::GetPath()
{
    HRESULT hr = DashLaunch::Init();
    if (FAILED(hr))
        return std::string();

    // Dashlaunch allows a maximum of 5 plugins
    const size_t maxNumberOfPlugins = 5;

    for (size_t i = 0; i <= maxNumberOfPlugins; i++)
    {
        char *pluginFullPath = nullptr;
        char pluginKey[8] = { 0 };

        // Create the plugin key, plugin1, plugin2 and so on
        _snprintf_s(pluginKey, _TRUNCATE, "plugin%d", i);

        // Get the path set for the current plugin key
        BOOL res = DashLaunch::GetOptionValueByName(pluginKey, reinterpret_cast<uint32_t *>(&pluginFullPath));

        // Some plugin keys might be missing in launch.ini so just continue to the next plugin if the key is not found
        if (res == FALSE)
            continue;

        // Extract the file name from the full plugin path
        char pluginFileName[MAX_PATH] = { 0 };
        _splitpath_s(
            pluginFullPath,
            nullptr, 0,
            nullptr, 0,
            pluginFileName, sizeof(pluginFileName),
            nullptr, 0
        );

        // If the current plugin file name is "Hayzen", return the current plugin full path
        if (!strcmp(pluginFileName, "Hayzen"))
            return pluginFullPath;
    }

    // If nothing is found, just return an empty string
    return std::string();
}

void Plugin::Update()
{
    while (m_Running)
    {
        uint32_t newTitleId = Xam::GetCurrentTitleId();
        if (newTitleId != m_CurrentTitleId)
            InitNewTitle(newTitleId);
    }
}

void Plugin::InitNewTitle(uint32_t newTitleId)
{
    // Clean up what previous game may have left out
    delete m_pCurrentTitle;

    m_CurrentTitleId = newTitleId;

    // Initialize the new game if it's supported
    // We have to check a string at a specific location to know if we are on the singleplayer or multiplayer XEX
    switch (newTitleId)
    {
    case TITLE_DASHBOARD:
        Dashboard::Init();
        break;
    case TITLE_MW2:
        if (!strcmp(reinterpret_cast<char *>(0x82001270), "multiplayer"))
            m_pCurrentTitle = new MW2Title();
        if (!strcmp(reinterpret_cast<char *>(0x8200EFE4), "startMultiplayer"))
            m_pCurrentTitle = new SpecOpsMW2Title();
        else if (!strcmp(reinterpret_cast<char *>(0x82001D38), "multiplayer"))
            m_pCurrentTitle = new AlphaMW2Title();
        else if (!strcmp(reinterpret_cast<char *>(0x8200EDA4), "startMultiplayer"))
            m_pCurrentTitle = new SpecOpsAlphaMW2Title();
        break;
    case TITLE_MW3:
        if (!strcmp(reinterpret_cast<char *>(0x82001458), "multiplayer"))
            m_pCurrentTitle = new MW3Title();
        if (!strcmp(reinterpret_cast<char *>(0x8200BEA8), "startMultiplayer"))
            m_pCurrentTitle = new SpecOpsMW3Title();
        break;
    default:
        break;
    }
}

// This wrapper is just here because CreateThread requires a C-style function pointer
uint32_t Plugin::UpdateThread(Plugin *This)
{
    This->Update();

    return 0;
}
