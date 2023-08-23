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

Plugin::Plugin(HANDLE pluginHandle)
    : m_Handle(pluginHandle), m_Running(true), m_Config("hdd:\\Hayzen.ini")
{
    // Start the main loop in a separate thread.
    // We use the extended version of Thread to create a thread that won't get stopped
    // when another game is launched.
    Memory::ThreadEx(reinterpret_cast<PTHREAD_START_ROUTINE>(Run), this, EXCREATETHREAD_SYSTEM);
}

Plugin::~Plugin()
{
    m_Running = false;

    // Wait a little bit for the system to clean things up before exiting the function
    Sleep(250);
}

uint32_t Plugin::Run(Plugin *This)
{
    // Wait a little bit for the console to be ready, this is especially necessary to
    // read the config file from disk and when the plugin is loaded by Dashlaunch on boot
    Sleep(1500);

    This->CreateConfig();

    while (This->m_Running)
    {
        uint32_t newTitleId = Xam::GetCurrentTitleId();
        if (newTitleId != This->m_CurrentTitleId)
            This->InitNewTitle(newTitleId);
    }

    return 0;
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

void Plugin::CreateConfig()
{
    // Get the full name from the handle
    LDR_DATA_TABLE_ENTRY *pDataTable = static_cast<LDR_DATA_TABLE_ENTRY *>(m_Handle);
    std::wstring widePath = pDataTable->FullDllName.Buffer;

    // If the plugin is not stored on HDD, just keep the default config path used
    // in the constructor initializer list
    std::wstring hddDevicePath = L"\\Device\\Harddisk0\\Partition1\\";
    size_t pos = widePath.find(hddDevicePath);
    if (pos == std::wstring::npos)
        return;

    // Only keep the absolute path on HDD
    // \Device\Harddisk0\Partition1\foo\bar\file => foo\bar\file
    widePath.erase(0, pos + hddDevicePath.size());

    std::string path = Formatter::ToNarrow(widePath);

    // Extract the directory from the path
    char pluginDirectory[MAX_PATH] = { 0 };
    _splitpath_s(
        path.c_str(),
        nullptr, 0,
        pluginDirectory, sizeof(pluginDirectory),
        nullptr, 0,
        nullptr, 0
    );

    // Rebuild the config file path from the path directory
    std::stringstream configFilePath;
    configFilePath << "hdd:\\";
    configFilePath << pluginDirectory;
    configFilePath << "Hayzen.ini";

    // This doesn't write the config file to disk, it just creates the in-memory object
    m_Config = Config(configFilePath.str());

    // Load the settings from the config (doesn't do anything if the config file doesn't exist)
    m_Config.Load();
}
