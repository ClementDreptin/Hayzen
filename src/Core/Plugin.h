#pragma once

#include "Core/Title.h"

class Plugin
{
public:
    Plugin(HANDLE pluginHandle);

    ~Plugin();

    HRESULT Init();

    void Run();

    std::string GetName();

    Fs::Path GetFullPath();

    std::string GetVersion();

private:
    HANDLE m_Handle;
    bool m_Running;
    uint32_t m_CurrentTitleId;
    Title *m_pCurrentTitle;

    void InitNewTitle(uint32_t newTitleId);

    HRESULT CreateConfig();

    HRESULT WaitUntilFilesystemIsReady();

    bool IsSingleplayerExecutable(uintptr_t stringAddress);

    bool IsMultiplayerExecutable(uintptr_t stringAddress);

    bool CheckKernelVersion();
};

extern Plugin *g_pPlugin;
