#pragma once

#include "Core/Title.h"

class Plugin
{
public:
    static void Start();

    static void Stop();

    static std::string GetPath();

private:
    static bool s_Running;
    static uint32_t s_CurrentTitleId;
    static Title *s_pCurrentTitle;

    static uint32_t Update(void *);

    static void InitNewTitle(uint32_t newTitleId);
};
