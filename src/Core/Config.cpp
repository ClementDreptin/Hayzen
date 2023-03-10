#include "pch.h"
#include "Core/Config.h"

#include "Core/Context.h"
#include "UI/Layout.h"

bool Config::s_HddMounted = false;

Config::Config(const std::string &filePath)
    : m_FilePath(filePath), m_ConfigFile(filePath)
{
}

bool Config::Save()
{
    // Mount the HDD the first time a config is saved
    if (!s_HddMounted)
        s_HddMounted = SUCCEEDED(Xam::MountHdd());

    // Reset the config
    m_Config.clear();

    // Populate the config from the menu settings
    m_Config["controls"]["showControls"] = Context::DisplayControlsTexts ? "true" : "false";
    m_Config["position"]["x"] = std::to_string(static_cast<long double>(Layout::X));
    m_Config["position"]["y"] = std::to_string(static_cast<long double>(Layout::Y));
    m_Config["color"]["r"] = std::to_string(static_cast<long long>(D3DCOLOR_GETRED(Layout::Color)));
    m_Config["color"]["g"] = std::to_string(static_cast<long long>(D3DCOLOR_GETGREEN(Layout::Color)));
    m_Config["color"]["b"] = std::to_string(static_cast<long long>(D3DCOLOR_GETBLUE(Layout::Color)));
    m_Config["color"]["a"] = std::to_string(static_cast<long long>(D3DCOLOR_GETALPHA(Layout::Color)));

    // Generate the config file
    return m_ConfigFile.generate(m_Config);
}
