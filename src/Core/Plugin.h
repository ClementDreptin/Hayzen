#pragma once

#include "Core/Title.h"

class Plugin
{
public:
    Plugin();

    ~Plugin();

    std::string GetPath();

private:
    bool m_Running;
    uint32_t m_CurrentTitleId;
    Title *m_pCurrentTitle;

    void Update();

    void InitNewTitle(uint32_t newTitleId);

    static uint32_t UpdateThread(Plugin *This);
};

extern Plugin *g_pPlugin;
