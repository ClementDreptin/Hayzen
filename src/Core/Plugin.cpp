#include "pch.h"
#include "Core/Plugin.h"

#include "Core/Config.h"
#include "Games/AlphaMW2/AlphaMW2Title.h"
#include "Games/MW2/MW2Title.h"
#include "Games/MW3/MW3Title.h"
#include "Games/NX1/NX1Title.h"
#include "Games/SpecOps/AlphaMW2/SpecOpsAlphaMW2Title.h"
#include "Games/SpecOps/MW2/SpecOpsMW2Title.h"
#include "Games/SpecOps/MW3/SpecOpsMW3Title.h"
#include "Modules/DebugEnabler.h"

Config g_Config("hdd:\\Hayzen.ini");

typedef enum _TitleId
{
    Title_Dashboard = 0xFFFE07D1,
    Title_XShell = 0xFFFE07FF,
    Title_MW2 = 0x41560817,
    Title_MW3 = 0x415608CB,
    Title_NX1 = 0x4156089E,
} TitleId;

Plugin::Plugin(HANDLE pluginHandle)
    : m_Handle(pluginHandle), m_Running(true), m_CurrentTitleId(0), m_pCurrentTitle(nullptr)
{
    LDR_DATA_TABLE_ENTRY *pDataTable = static_cast<LDR_DATA_TABLE_ENTRY *>(m_Handle);
    m_Name = Formatter::ToNarrow(pDataTable->BaseDllName.Buffer);

    // Start the main loop in a separate thread.
    // We use the extended version of Thread to create a thread that won't get stopped
    // when another game is launched.
    Memory::ThreadEx(reinterpret_cast<PTHREAD_START_ROUTINE>(Run), this, EXCREATETHREAD_SYSTEM);
}

Plugin::~Plugin()
{
    m_Running = false;

    if (g_Config.AllowDebugBuilds && !Xam::IsDevkit())
        DebugEnabler::Disable();

    delete m_pCurrentTitle;

    // Wait a little bit for the system to clean things up before exiting the function
    Sleep(250);
}

HRESULT Plugin::SaveConfig()
{
    // It is necessary mount the HDD again because this function might get called from a game
    // which my not have the HDD mounted
    Xam::MountHdd();

    return g_Config.SaveToDisk();
}

void Plugin::Init()
{
    CreateConfig();

    if (g_Config.AllowDebugBuilds && !Xam::IsDevkit())
    {
        HRESULT hr = DebugEnabler::Enable();
        if (FAILED(hr))
            Xam::XNotify("Couldn't enable debug builds", Xam::XNOTIFYUI_TYPE_AVOID_REVIEW);
    }
}

uint32_t Plugin::Run(Plugin *This)
{
    // Wait a little bit for the console to be ready, this is especially necessary to
    // read the config file from disk and when the plugin is loaded by Dashlaunch on boot
    Sleep(1500);

    This->Init();

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
    case Title_Dashboard:
        Xam::XNotify("Hayzen - Dashboard Detected");
        m_pCurrentTitle = nullptr;
        break;
    case Title_XShell:
        Xam::XNotify("Hayzen - XShell Detected");
        m_pCurrentTitle = nullptr;
        break;
    case Title_MW2:
        if (!strcmp(reinterpret_cast<char *>(0x82001270), "multiplayer"))
            m_pCurrentTitle = new MW2Title();
        else if (!strcmp(reinterpret_cast<char *>(0x8200EFE4), "startMultiplayer"))
            m_pCurrentTitle = new SpecOpsMW2Title();
        else if (!strcmp(reinterpret_cast<char *>(0x82001D38), "multiplayer"))
            m_pCurrentTitle = new AlphaMW2Title();
        else if (!strcmp(reinterpret_cast<char *>(0x8200EDA4), "startMultiplayer"))
            m_pCurrentTitle = new SpecOpsAlphaMW2Title();
        break;
    case Title_MW3:
        if (!strcmp(reinterpret_cast<char *>(0x82001458), "multiplayer"))
            m_pCurrentTitle = new MW3Title();
        else if (!strcmp(reinterpret_cast<char *>(0x8200BEA8), "startMultiplayer"))
            m_pCurrentTitle = new SpecOpsMW3Title();
        break;
    case Title_NX1:
        if (!strcmp(reinterpret_cast<char *>(0x820023A0), "multiplayer"))
            m_pCurrentTitle = new NX1Title();
        break;
    default:
        m_pCurrentTitle = nullptr;
        break;
    }
}

void Plugin::CreateConfig()
{
    // Allow access to HDD
    Xam::MountHdd();

    // Get the full name from the handle
    LDR_DATA_TABLE_ENTRY *pDataTable = static_cast<LDR_DATA_TABLE_ENTRY *>(m_Handle);
    std::wstring widePath = pDataTable->FullDllName.Buffer;

    // If the plugin is not stored on HDD, just keep the default config path used
    // in the constructor initializer list
    std::wstring hddDevicePath = L"\\Device\\Harddisk0\\Partition1\\";
    size_t pos = widePath.find(hddDevicePath);
    if (pos == std::wstring::npos)
    {
        DebugPrint(
            L"[Hayzen][Config]: Warn: %s is not stored on HDD so can't store config next to it."
            L"The config will be written to the default location (root of HDD) when the settings are saved.",
            pDataTable->BaseDllName.Buffer
        );
        return;
    }

    // Only keep the absolute path on HDD
    // \Device\Harddisk0\Partition1\foo\bar\file => foo\bar\file
    widePath.erase(0, pos + hddDevicePath.size());

    std::string path = Formatter::ToNarrow(widePath);

    // Extract the directory from the path
    char pluginDirectory[MAX_PATH] = {};
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
    g_Config = Config(configFilePath.str());

    // This doesn't do anything if the config file doesn't exist
    g_Config.LoadFromDisk();
}
