#include "pch.h"
#include "Core/Config.h"

#include "Core/Context.h"
#include "UI/Layout.h"
#include "UI/Renderer.h"

bool Config::s_HddMounted = false;

Config::Config(const std::string &filePath)
    : m_FilePath(filePath), m_ConfigFile(filePath)
{
}

bool Config::Save()
{
    MountHddIfNeeded();

    // Populate the config from the menu settings
    m_Config["controls"]["showcontrols"] = Context::DisplayControlsTexts ? "true" : "false";
    m_Config["position"]["x"] = std::to_string(static_cast<long double>(Layout::X));
    m_Config["position"]["y"] = std::to_string(static_cast<long double>(Layout::Y));
    m_Config["color"]["r"] = std::to_string(static_cast<long long>(D3DCOLOR_GETRED(Layout::Color)));
    m_Config["color"]["g"] = std::to_string(static_cast<long long>(D3DCOLOR_GETGREEN(Layout::Color)));
    m_Config["color"]["b"] = std::to_string(static_cast<long long>(D3DCOLOR_GETBLUE(Layout::Color)));
    m_Config["color"]["a"] = std::to_string(static_cast<long long>(D3DCOLOR_GETALPHA(Layout::Color)));

    // Generate the config file
    return m_ConfigFile.generate(m_Config);
}

bool Config::Load()
{
    MountHddIfNeeded();

    // Load the config from disk
    bool canReadFile = m_ConfigFile.read(m_Config);
    if (!canReadFile)
        return false;

    // Controls
    if (m_Config.has("controls"))
    {
        if (m_Config["controls"].has("showcontrols"))
            Context::DisplayControlsTexts = m_Config["controls"]["showcontrols"] == "true";
    }

    // Position
    if (m_Config.has("position"))
    {
        if (m_Config["position"].has("x"))
            Layout::X = static_cast<float>(atof(m_Config["position"]["x"].c_str()));

        if (m_Config["position"].has("y"))
            Layout::Y = static_cast<float>(atof(m_Config["position"]["y"].c_str()));
    }

    // Color
    if (m_Config.has("color"))
    {
        if (m_Config["color"].has("r"))
            Layout::Color = D3DCOLOR_RED(Layout::Color, atoi(m_Config["color"]["r"].c_str()));

        if (m_Config["color"].has("g"))
            Layout::Color = D3DCOLOR_GREEN(Layout::Color, atoi(m_Config["color"]["g"].c_str()));

        if (m_Config["color"].has("b"))
            Layout::Color = D3DCOLOR_BLUE(Layout::Color, atoi(m_Config["color"]["b"].c_str()));

        if (m_Config["color"].has("a"))
            Layout::Color = D3DCOLOR_ALPHA(Layout::Color, atoi(m_Config["color"]["a"].c_str()));
    }

    return true;
}

void Config::MountHddIfNeeded()
{
    if (!s_HddMounted)
        s_HddMounted = SUCCEEDED(Xam::MountHdd());
}
