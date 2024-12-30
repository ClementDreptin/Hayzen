#pragma once

#include "Core/Title.h"

class Plugin
{
public:
    Plugin(HANDLE pluginHandle);

    ~Plugin();

    const std::string &GetName() const { return m_Name; }

    HRESULT SaveConfig();

private:
    HANDLE m_Handle;
    std::string m_Name;
    bool m_Running;
    uint32_t m_CurrentTitleId;
    Title *m_pCurrentTitle;

    void Init();

    static uint32_t Run(Plugin *This);

    void InitNewTitle(uint32_t newTitleId);

    void CreateConfig();
};

extern Plugin *g_pPlugin;
