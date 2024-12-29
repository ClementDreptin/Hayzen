#include "pch.h"
#include "Core/Config.h"

#include "Core/Settings.h"
#include "Core/UI.h"

Config::Config(const std::string &filePath)
    : m_ConfigFile(filePath)
{
}

HRESULT Config::Save()
{
    m_Config["debugbuilds"]["allowdebugbuilds"] = Settings::AllowDebugBuilds ? "true" : "false";

    m_Config["controls"]["showcontrols"] = Settings::DisplayControlsTexts ? "true" : "false";

    m_Config["carepackage"]["distance"] = std::to_string(static_cast<long double>(Settings::CarePackageDistance));
    m_Config["carepackage"]["height"] = std::to_string(static_cast<long double>(Settings::CarePackageHeight));
    m_Config["carepackage"]["angle"] = std::to_string(static_cast<long double>(Settings::CarePackageAngle));

    m_Config["position"]["x"] = std::to_string(static_cast<long double>(Settings::X));
    m_Config["position"]["y"] = std::to_string(static_cast<long double>(Settings::Y));

    m_Config["color"]["r"] = std::to_string(static_cast<uint64_t>(D3DCOLOR_GETRED(Settings::Color)));
    m_Config["color"]["g"] = std::to_string(static_cast<uint64_t>(D3DCOLOR_GETGREEN(Settings::Color)));
    m_Config["color"]["b"] = std::to_string(static_cast<uint64_t>(D3DCOLOR_GETBLUE(Settings::Color)));
    m_Config["color"]["a"] = std::to_string(static_cast<uint64_t>(D3DCOLOR_GETALPHA(Settings::Color)));

    bool success = m_ConfigFile.generate(m_Config);

#ifndef NDEBUG
    if (!success)
        DebugPrint("[Hayzen][Config]: Warn: Could not write config to disk.");
#endif

    return success ? S_OK : E_FAIL;
}

HRESULT Config::Load()
{
    // Load the config from disk
    bool canReadFile = m_ConfigFile.read(m_Config);
    if (!canReadFile)
    {
        DebugPrint("[Hayzen][Config]: Warn: Could not read config from disk.");
        return E_FAIL;
    }

    // Debug builds
    if (m_Config.has("debugbuilds"))
    {
        if (m_Config["debugbuilds"].has("allowdebugbuilds"))
            Settings::AllowDebugBuilds = m_Config["debugbuilds"]["allowdebugbuilds"] == "true";
    }

    // Controls
    if (m_Config.has("controls"))
    {
        if (m_Config["controls"].has("showcontrols"))
            Settings::DisplayControlsTexts = m_Config["controls"]["showcontrols"] == "true";
    }

    // Care package
    if (m_Config.has("carepackage"))
    {
        if (m_Config["carepackage"].has("distance"))
            Settings::CarePackageDistance = static_cast<float>(atof(m_Config["carepackage"]["distance"].c_str()));

        if (m_Config["carepackage"].has("height"))
            Settings::CarePackageHeight = static_cast<float>(atof(m_Config["carepackage"]["height"].c_str()));

        if (m_Config["carepackage"].has("angle"))
            Settings::CarePackageAngle = static_cast<float>(atof(m_Config["carepackage"]["angle"].c_str()));
    }

    // Position
    if (m_Config.has("position"))
    {
        if (m_Config["position"].has("x"))
            Settings::X = static_cast<float>(atof(m_Config["position"]["x"].c_str()));

        if (m_Config["position"].has("y"))
            Settings::Y = static_cast<float>(atof(m_Config["position"]["y"].c_str()));
    }

    // Color
    if (m_Config.has("color"))
    {
        if (m_Config["color"].has("r"))
            Settings::Color = D3DCOLOR_RED(Settings::Color, atoi(m_Config["color"]["r"].c_str()));

        if (m_Config["color"].has("g"))
            Settings::Color = D3DCOLOR_GREEN(Settings::Color, atoi(m_Config["color"]["g"].c_str()));

        if (m_Config["color"].has("b"))
            Settings::Color = D3DCOLOR_BLUE(Settings::Color, atoi(m_Config["color"]["b"].c_str()));

        if (m_Config["color"].has("a"))
            Settings::Color = D3DCOLOR_ALPHA(Settings::Color, atoi(m_Config["color"]["a"].c_str()));
    }

    return S_OK;
}
