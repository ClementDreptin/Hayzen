#pragma once

#include "Core/Title.h"

class Plugin
{
public:
    Plugin(HANDLE pluginHandle);

    ~Plugin();

    std::string GetName();

    std::string GetFullPath();

    std::string GetVersion();

    HRESULT SaveConfig();

private:
    HANDLE m_Handle;
    HANDLE m_RunThreadHandle;
    bool m_Running;
    uint32_t m_CurrentTitleId;
    Title *m_pCurrentTitle;

    void Init();

    static uint32_t Run(Plugin *This);

    void InitNewTitle(uint32_t newTitleId);

    HRESULT CreateConfig();

    HRESULT WaitUntilFilesystemIsReady();

    bool IsSingleplayerExecutable(uintptr_t stringAddress);

    bool IsMultiplayerExecutable(uintptr_t stringAddress);
};

extern Plugin *g_pPlugin;
