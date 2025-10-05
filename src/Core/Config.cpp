#include "pch.h"
#include "Core/Config.h"

#include "Core/UI.h"

Config::Config(const Fs::Path &filePath)
    : AllowDebugBuilds(true),
      DisplayControlsTexts(true),
      AutoUpdate(true),
      Y(150.0f),
      Color(D3DCOLOR_XRGB(210, 20, 20)),
      BackgroundColor(D3DCOLOR_XRGB(25, 25, 25)),
      TextColor(D3DCOLOR_XRGB(255, 255, 255)),
      Padding(5.0f),
      Gap(2.0f),
      BorderWidth(2.5f),

      // The following values are calculated dynamically when initializing the menu
      X(0.0f),
      Width(0.0f),
      Height(0.0f),

      // Private member
      m_File(filePath.String())
{
}

HRESULT Config::SaveToDisk()
{
    m_Structure["debugbuilds"]["allowdebugbuilds"] = AllowDebugBuilds ? "true" : "false";

    m_Structure["controls"]["showcontrols"] = DisplayControlsTexts ? "true" : "false";

    m_Structure["update"]["autoupdate"] = AutoUpdate ? "true" : "false";

    m_Structure["position"]["x"] = std::to_string(static_cast<long double>(X));
    m_Structure["position"]["y"] = std::to_string(static_cast<long double>(Y));

    m_Structure["color"]["r"] = std::to_string(static_cast<uint64_t>(D3DCOLOR_GETRED(Color)));
    m_Structure["color"]["g"] = std::to_string(static_cast<uint64_t>(D3DCOLOR_GETGREEN(Color)));
    m_Structure["color"]["b"] = std::to_string(static_cast<uint64_t>(D3DCOLOR_GETBLUE(Color)));
    m_Structure["color"]["a"] = std::to_string(static_cast<uint64_t>(D3DCOLOR_GETALPHA(Color)));

    bool success = m_File.generate(m_Structure);

#ifndef NDEBUG
    if (!success)
        DebugPrint("[Hayzen][Config]: Warn: Could not write config to disk.");
#endif

    return success ? S_OK : E_FAIL;
}

HRESULT Config::LoadFromDisk()
{
    // Load the config from disk
    bool canReadFile = m_File.read(m_Structure);
    if (!canReadFile)
    {
        DebugPrint("[Hayzen][Config]: Warn: Could not read config from disk.");
        return E_FAIL;
    }

    // Debug builds
    if (m_Structure.has("debugbuilds"))
    {
        if (m_Structure["debugbuilds"].has("allowdebugbuilds"))
            AllowDebugBuilds = m_Structure["debugbuilds"]["allowdebugbuilds"] == "true";
    }

    // Controls
    if (m_Structure.has("controls"))
    {
        if (m_Structure["controls"].has("showcontrols"))
            DisplayControlsTexts = m_Structure["controls"]["showcontrols"] == "true";
    }

    // Auto update
    if (m_Structure.has("update"))
    {
        if (m_Structure["update"].has("autoupdate"))
            AutoUpdate = m_Structure["update"]["autoupdate"] == "true";
    }

    // Position
    if (m_Structure.has("position"))
    {
        if (m_Structure["position"].has("x"))
            X = static_cast<float>(atof(m_Structure["position"]["x"].c_str()));

        if (m_Structure["position"].has("y"))
            Y = static_cast<float>(atof(m_Structure["position"]["y"].c_str()));
    }

    // Color
    if (m_Structure.has("color"))
    {
        if (m_Structure["color"].has("r"))
            Color = D3DCOLOR_RED(Color, atoi(m_Structure["color"]["r"].c_str()));

        if (m_Structure["color"].has("g"))
            Color = D3DCOLOR_GREEN(Color, atoi(m_Structure["color"]["g"].c_str()));

        if (m_Structure["color"].has("b"))
            Color = D3DCOLOR_BLUE(Color, atoi(m_Structure["color"]["b"].c_str()));

        if (m_Structure["color"].has("a"))
            Color = D3DCOLOR_ALPHA(Color, atoi(m_Structure["color"]["a"].c_str()));
    }

    return S_OK;
}

void Config::Reset()
{
    // Double cast to round to the closest integer value
    X = static_cast<float>(static_cast<uint32_t>(UI::DisplayWidth - Width - 10.0f));
    Y = 150.0f;
    Color = D3DCOLOR_XRGB(210, 20, 20);
    DisplayControlsTexts = true;
}
