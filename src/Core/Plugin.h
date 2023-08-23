#pragma once

#include "Core/Config.h"
#include "Core/Title.h"

class Plugin
{
public:
    Plugin(HANDLE pluginHandle);

    ~Plugin();

    Config &GetConfig() { return m_Config; }

private:
    HANDLE m_Handle;
    bool m_Running;
    uint32_t m_CurrentTitleId;
    Title *m_pCurrentTitle;
    Config m_Config;

    static uint32_t Run(Plugin *This);

    void InitNewTitle(uint32_t newTitleId);

    void CreateConfig();
};

extern Plugin *g_pPlugin;
