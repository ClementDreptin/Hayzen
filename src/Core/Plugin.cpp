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
#include "Modules/AutoUpdater.h"
#include "Modules/DebugEnabler.h"
#include "Modules/NotificationPatcher.h"

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
    // Start the main loop in a separate thread.
    // We use the extended version of Thread to create a thread that won't get stopped
    // when another game is launched.
    m_RunThreadHandle = ThreadEx(
        reinterpret_cast<PTHREAD_START_ROUTINE>(Run),
        this,
        EXCREATETHREAD_FLAG_SYSTEM
    );
}

Plugin::~Plugin()
{
    m_Running = false;

    // Disable debug builds if needed
    if (g_Config.AllowDebugBuilds && !IsDevkit())
        DebugEnabler::Disable();

    // Unload the notification patch
    NotificationPatcher::Disable();

    // Cleanup the currently running title
    delete m_pCurrentTitle;

    // Wait for the run thread to finish
    WaitForSingleObject(m_RunThreadHandle, INFINITE);
    CloseHandle(m_RunThreadHandle);
}

std::string Plugin::GetName()
{
    LDR_DATA_TABLE_ENTRY *pDataTable = static_cast<LDR_DATA_TABLE_ENTRY *>(m_Handle);

    return Formatter::ToNarrow(pDataTable->BaseDllName.Buffer);
}

Fs::Path Plugin::GetFullPath()
{
    LDR_DATA_TABLE_ENTRY *pDataTable = static_cast<LDR_DATA_TABLE_ENTRY *>(m_Handle);

    return Formatter::ToNarrow(pDataTable->FullDllName.Buffer);
}

std::string Plugin::GetVersion()
{
    LDR_DATA_TABLE_ENTRY *pDataTable = static_cast<LDR_DATA_TABLE_ENTRY *>(m_Handle);
    XEX_EXECUTION_ID *pExecutionId = static_cast<XEX_EXECUTION_ID *>(
        RtlImageXexHeaderField(pDataTable->XexHeaderBase, XEX_HEADER_FIELD_EXECUTION_ID)
    );

    return Formatter::Format(
        "v%hhu.%hhu.%hhu",
        pExecutionId->Version.Major,
        pExecutionId->Version.Minor,
        pExecutionId->Version.Qfe
    );
}

HRESULT Plugin::SaveConfig()
{
    // It is necessary mount the HDD again because this function might get called from a game
    // which my not have the HDD mounted
    HRESULT hr = Fs::MountHdd();
    if (FAILED(hr) && hr != STATUS_OBJECT_NAME_COLLISION)
    {
        DebugPrint("[Hayzen][Config]: Error: Couldn't mount HDD: %X.", hr);
        return hr;
    }

    return g_Config.SaveToDisk();
}

HRESULT Plugin::Init()
{
    // Check if we're running in a supported environment
    bool onSupportedKernel = CheckKernelVersion();
    if (!onSupportedKernel)
        return E_FAIL;

    // Setup config.
    // We explicit discard potential errors because the config isn't absolutely necessary
    // to use the plugin
    CreateConfig();

    // Allow notifications to be displayed from system threads
    NotificationPatcher::Enable();

    // Enable debug builds if needed
    if (g_Config.AllowDebugBuilds && !IsDevkit())
    {
        HRESULT hr = DebugEnabler::Enable();
        if (FAILED(hr))
            Xam::XNotify("Couldn't enable debug builds", Xam::XNOTIFYUI_TYPE_AVOID_REVIEW);
    }

    // Run the auto updater if needed
    if (g_Config.AutoUpdate)
        AutoUpdater::Run();

    return S_OK;
}

uint32_t Plugin::Run(Plugin *This)
{
    HRESULT hr = This->Init();
    if (FAILED(hr))
        return 1;

    while (This->m_Running)
    {
        uint32_t newTitleId = XamGetCurrentTitleId();
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

    // Allow access to HDD.
    // Collisions are expected because, when running on a console with Dashlaunch, there is
    // already a system symlink named "hdd:". Collisions can also happen when loading the plugin
    // multiple times because system symlinks continue to live after the process terminates.
    hr = Fs::MountHdd();
    if (FAILED(hr) && hr != STATUS_OBJECT_NAME_COLLISION)
    {
        DebugPrint("[Hayzen][Config]: Error: Couldn't mount HDD: %X.", hr);
        return hr;
    }

    // Get the plugin NT device path from the handle
    Fs::Path pluginDevicePath = GetFullPath();

    // If the plugin is not stored on HDD, just keep the default config path used
    // in the constructor initializer list
    std::string hddDevicePath = "\\Device\\Harddisk0\\Partition1\\";
    size_t pos = pluginDevicePath.String().find(hddDevicePath);
    if (pos != 0)
    {
        DebugPrint(
            "[Hayzen][Config]: Warn: %s is not stored on HDD so can't store config next to it."
            "The config will be written to the default location (root of HDD) when the settings are saved.",
            pluginDevicePath.c_str()
        );

        g_Config.LoadFromDisk();

        return hr;
    }

    // Only keep the path relative to the HDD root
    // \Device\Harddisk0\Partition1\foo\bar\file => foo\bar\file
    Fs::Path pluginPathRelativeToHddRoot = pluginDevicePath.String().substr(hddDevicePath.size());

    // Rebuild the config file path from the path directory
    Fs::Path configFilePath = "hdd:" / pluginPathRelativeToHddRoot.Parent() / "Hayzen.ini";

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

        return hr;
    }

    return hr;
}

HRESULT Plugin::WaitUntilFilesystemIsReady()
{
    Fs::Path path = GetFullPath();

    // Create the attributes from the path
    OBJECT_ATTRIBUTES attributes = {};
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
        IO_STATUS_BLOCK block = {};
        NTSTATUS status = NtOpenFile(
            &handle,
            FILE_READ_ATTRIBUTES,
            &attributes,
            &block,
            FILE_SHARE_READ,
            0
        );

        // If we managed to read the file, stop here
        if (NT_SUCCESS(status) || handle != INVALID_HANDLE_VALUE)
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

bool Plugin::CheckKernelVersion()
{
    bool onSupportedKernel = false;

    // When using the newer versions of RGLoader, XboxKrnlVersion->Build is spoofed to
    // return 17559 even though the system binaries (xboxkrnld.exe, xam.xex, etc.) are
    // the 17489 versions
    if (IsDevkit())
        onSupportedKernel = XboxKrnlVersion->Build == 17489 || XboxKrnlVersion->Build == 17559;
    else
        onSupportedKernel = XboxKrnlVersion->Build == 17559;

    // Display an error if the current kernel version is not supported
    if (!onSupportedKernel)
    {
        std::vector<std::wstring> buttonLabels(1);
        buttonLabels[0] = L"OK";

        std::wstring errorMessage = Formatter::Format(
            L"Your dashboard version is not supported.\n\n"
            L"Supported versions:\n"
            L"  Retail: 17559\n"
            L"  Devkit: 17489 (or 17559 RGLoader spoof)\n\n"
            L"Your version: %d",
            XboxKrnlVersion->Build
        );

        Xam::ShowMessageBox(
            L"Unsupported dashboard",
            errorMessage,
            buttonLabels,
            XMB_ERRORICON
        );
    }

    return onSupportedKernel;
}
