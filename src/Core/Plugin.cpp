#include "pch.h"
#include "Core/Plugin.h"

#include "Core/Config.h"
#include "Games/AlphaGhosts/AlphaGhostsTitle.h"
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
    Title_AlphaGhosts = 0x4156088E,
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
        if (IsMultiplayerExecutable(0x82001270))
            m_pCurrentTitle = new MW2Title();
        else if (IsSingleplayerExecutable(0x8200EFE4))
            m_pCurrentTitle = new SpecOpsMW2Title();
        else if (IsMultiplayerExecutable(0x82001D38))
            m_pCurrentTitle = new AlphaMW2Title();
        else if (IsSingleplayerExecutable(0x8200EDA4))
            m_pCurrentTitle = new SpecOpsAlphaMW2Title();
        break;
    case Title_MW3:
        if (IsMultiplayerExecutable(0x82001458))
            m_pCurrentTitle = new MW3Title();
        else if (IsSingleplayerExecutable(0x8200BEA8))
            m_pCurrentTitle = new SpecOpsMW3Title();
        break;
    case Title_AlphaGhosts:
        if (IsMultiplayerExecutable(0x820029B0))
            m_pCurrentTitle = new AlphaGhostsTitle();
        break;
    case Title_NX1:
        if (IsMultiplayerExecutable(0x820023A0))
            m_pCurrentTitle = new NX1Title();
        break;
    default:
        m_pCurrentTitle = nullptr;
        break;
    }
}

HRESULT Plugin::CreateConfig()
{
    HRESULT hr = S_OK;

    // When loading the plugin on boot with Dashlaunch, the filesystem might not be yet ready when
    // we get here
    hr = WaitUntilFilesystemIsReady();
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][Config]: Error: Filesystem never became ready: %X.", hr);
        return hr;
    }

    // Allow access to HDD
    hr = Xam::MountHdd();
    if (FAILED(hr))
    {
        DebugPrint("[Hayzen][Config]: Error: Couldn't mount HDD: %X.", hr);
        return hr;
    }

    // Get the plugin path from the handle
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

        g_Config.LoadFromDisk();

        return hr;
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
    std::string configFilePath = Formatter::Format("hdd:\\%sHayzen.ini", pluginDirectory);

    // This doesn't write the config file to disk, it just creates the in-memory object
    g_Config = Config(configFilePath);

    // Load the config from disk
    hr = g_Config.LoadFromDisk();
    if (FAILED(hr))
    {
        DebugPrint(
            "[Hayzen][Config]: Warn: The config file (%s) couldn't be loaded from disk."
            "Loading the config isn't necessary to run the plugin.",
            configFilePath.c_str()
        );

        // We explicitely return a success code here because it's fine if the user never created a config
        return S_OK;
    }

    return hr;
}

HRESULT Plugin::WaitUntilFilesystemIsReady()
{
    // Get the plugin path from the handle
    LDR_DATA_TABLE_ENTRY *pDataTable = static_cast<LDR_DATA_TABLE_ENTRY *>(m_Handle);
    std::string path = Formatter::ToNarrow(pDataTable->FullDllName.Buffer);

    // Create the attributes from the path
    OBJECT_ATTRIBUTES attributes;
    STRING pathString = {};
    RtlInitAnsiString(&pathString, path.c_str());
    InitializeObjectAttributes(&attributes, &pathString, 0, nullptr);

    // Try to use the filesystem multiple times until it works
    const size_t maxRetries = 10;
    for (size_t i = 0; i < maxRetries; i++)
    {
        // We use the kernel function to open the file because the path is an
        // NT device path (e.g. \Device\Harddisk0\Partition1\...), which means it
        // can't be accessed using user-mode APIs, like the standard library
        HANDLE handle = INVALID_HANDLE_VALUE;
        IO_STATUS_BLOCK block;
        NTSTATUS status = NtOpenFile(
            &handle,
            FILE_READ_ATTRIBUTES,
            &attributes,
            &block,
            FILE_SHARE_READ,
            0
        );

        // If we managed to read the file, stop here
        if (NT_SUCCESS(status) || handle == INVALID_HANDLE_VALUE)
        {
            NtClose(handle);

            return S_OK;
        }

        // Wait a little bit before trying again
        Sleep(100);
    }

    return E_FAIL;
}

bool Plugin::IsSingleplayerExecutable(uintptr_t stringAddress)
{
    // If the string "startMultiplayer" is present at stringAddress, it means we are on the
    // correct singleplayer executable
    const char singleplayerStr[] = "startMultiplayer";

    return strncmp(reinterpret_cast<const char *>(stringAddress), singleplayerStr, sizeof(singleplayerStr)) == 0;
}

bool Plugin::IsMultiplayerExecutable(uintptr_t stringAddress)
{
    // If the string "multiplayer" is present at stringAddress, it means we are on the
    // correct multiplayer executable
    const char multiplayerStr[] = "multiplayer";

    return strncmp(reinterpret_cast<const char *>(stringAddress), multiplayerStr, sizeof(multiplayerStr)) == 0;
}
