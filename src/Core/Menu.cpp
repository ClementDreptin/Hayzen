#include "pch.h"
#include "Core/Menu.h"

#include "Core/Config.h"
#include "Core/Plugin.h"
#include "Core/UI.h"
#include "Modules/DebugEnabler.h"
#include "Options/ClickOption.h"
#include "Options/ColorPickerOption.h"
#include "Options/RangeOption.h"
#include "Options/SubOptionGroup.h"
#include "Options/ToggleOption.h"

Menu::Menu()
    : m_Open(false), m_CurrentOptionGroupIndex(0), m_CachedOptionGroupHeadersHeight(0.0f)
{
}

void Menu::Init(const std::vector<OptionGroup> &optionGroups)
{
    m_OptionGroups = optionGroups;

    AddSettingsGroup();

    CalculateMenuDimensions();
}

void Menu::Update(Input::Gamepad *pGamepad)
{
    XASSERT(pGamepad != nullptr);

    // If the menu is not open, no need to go further
    if (!m_Open)
        return;

    // Allow the user to change the current option group with LB/RB
    if (pGamepad->PressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
    {
        if (m_CurrentOptionGroupIndex > 0)
            m_CurrentOptionGroupIndex--;
    }
    else if (pGamepad->PressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
    {
        if (m_CurrentOptionGroupIndex < m_OptionGroups.size() - 1)
            m_CurrentOptionGroupIndex++;
    }

    m_OptionGroups[m_CurrentOptionGroupIndex].Update(pGamepad);
}

void Menu::Render()
{
    if (g_Config.DisplayControlsTexts)
        RenderControlsTexts();

    // If the menu is not open, no need to go further
    if (!m_Open)
        return;

    RenderOptionGroupHeaders();

    float optionGroupHeadersHeight = GetOptionGroupHeadersHeight();
    m_OptionGroups[m_CurrentOptionGroupIndex].Render(g_Config.X, g_Config.Y + optionGroupHeadersHeight, g_Config.Width, g_Config.Height);
}

void Menu::AddSettingsGroup()
{
    std::vector<std::shared_ptr<Option>> options;

    options.emplace_back(MakeOption(ToggleOption, "Show Controls", &g_Config.DisplayControlsTexts));

    if (!Xam::IsDevkit())
        options.emplace_back(MakeOption(ToggleOption, "Allow Debug Builds", [this](void *params) { return ToggleDebugBuilds(params); }, &g_Config.AllowDebugBuilds));

    std::vector<std::shared_ptr<Option>> menuPositionOptions;
    menuPositionOptions.emplace_back(MakeOption(RangeOption<float>, "X", &g_Config.X, g_Config.BorderWidth, UI::DisplayWidth, 10.0f));
    menuPositionOptions.emplace_back(MakeOption(RangeOption<float>, "Y", &g_Config.Y, g_Config.BorderWidth, UI::DisplayHeight, 10.0f));
    options.emplace_back(MakeOption(SubOptionGroup, "Menu Position", menuPositionOptions));

    options.emplace_back(MakeOption(ColorPickerOption, "Menu Color", &g_Config.Color));
    options.emplace_back(MakeOption(ClickOption, "Save Settings", [this](void *params) { return SaveSettings(params); }));
    options.emplace_back(MakeOption(ClickOption, "Reset Settings", [this](void *params) { return ResetSettings(params); }));
    m_OptionGroups.emplace_back(OptionGroup("Settings", options));
}

float Menu::GetOptionGroupHeadersHeight() const
{
    // Return the cached value if the minimum width has already been calculated
    if (m_CachedOptionGroupHeadersHeight != 0.0f)
        return m_CachedOptionGroupHeadersHeight;

    float m_CachedOptionGroupHeadersHeight = 0.0f;
    for (size_t i = 0; i < m_OptionGroups.size(); i++)
    {
        float currentOptionGroupHeaderHeight = UI::GetTextHeight(m_OptionGroups[i].GetName()) + g_Config.Padding * 2;
        if (currentOptionGroupHeaderHeight > m_CachedOptionGroupHeadersHeight)
            m_CachedOptionGroupHeadersHeight = currentOptionGroupHeaderHeight;
    }
    m_CachedOptionGroupHeadersHeight += g_Config.BorderWidth;

    return m_CachedOptionGroupHeadersHeight;
}

void Menu::RenderOptionGroupHeaders()
{
    float optionGroupHeadersHeight = GetOptionGroupHeadersHeight();

    for (size_t i = 0; i < m_OptionGroups.size(); i++)
    {
        // The X offset is the sum of the previous option group header widths
        float offset = g_Config.X;
        for (size_t j = 0; j < i; j++)
            offset += (UI::GetTextWidth(m_OptionGroups[j].GetName()) + g_Config.Padding * 2 + g_Config.BorderWidth);

        UI::TextProps props = {};
        props.X = offset;
        props.Y = g_Config.Y;
        props.Text = m_OptionGroups[i].GetName();
        props.BackgroundHeight = optionGroupHeadersHeight;
        props.BorderWidth = g_Config.BorderWidth;

        // Make the header more transparent when the option group header is not selected
        uint8_t alpha = 100;
        props.Color = i == m_CurrentOptionGroupIndex ? g_Config.TextColor : D3DCOLOR_ALPHA(g_Config.TextColor, alpha);
        props.BackgroundColor = i == m_CurrentOptionGroupIndex ? g_Config.BackgroundColor : D3DCOLOR_ALPHA(g_Config.BackgroundColor, alpha);
        props.BorderColor = i == m_CurrentOptionGroupIndex ? g_Config.Color : D3DCOLOR_ALPHA(g_Config.Color, alpha);

        // m_OptionGroups[i] is selected
        if (i == m_CurrentOptionGroupIndex)
            props.BorderPosition = static_cast<UI::BorderPosition>(UI::Border_Left | UI::Border_Top | UI::Border_Right);

        // m_OptionGroups[i] is to the left of the selected option group
        else if (i < m_CurrentOptionGroupIndex)
            props.BorderPosition = static_cast<UI::BorderPosition>(UI::Border_Left | UI::Border_Top);

        // m_OptionGroups[i] is to the right of the selected option group
        else if (i > m_CurrentOptionGroupIndex)
            props.BorderPosition = static_cast<UI::BorderPosition>(UI::Border_Right | UI::Border_Top);

        UI::DrawText(props);
    }
}

void Menu::CalculateMenuDimensions()
{
    float allOptionGroupNamesWidth = 0.0f;
    float biggestOptionGroupWidth = 0.0f;
    float biggestOptionGroupHeight = 0.0f;

    for (size_t i = 0; i < m_OptionGroups.size(); i++)
    {
        // Accumulate all the option group names width
        allOptionGroupNamesWidth += (UI::GetTextWidth(m_OptionGroups[i].GetName()) + g_Config.Padding * 2);

        // Find the option group with the biggest width
        float optionGroupWidth = m_OptionGroups[i].GetMinWidth();
        if (biggestOptionGroupWidth < optionGroupWidth)
            biggestOptionGroupWidth = optionGroupWidth;

        // Find the option group with the biggest height
        float optionGroupHeight = m_OptionGroups[i].GetMinHeight();
        if (biggestOptionGroupHeight < optionGroupHeight)
            biggestOptionGroupHeight = optionGroupHeight;
    }

    // Take the space between each group header into account
    allOptionGroupNamesWidth += g_Config.BorderWidth * (m_OptionGroups.size() - 1);

    // Make the menu width the max between all the option names width and the longest option name
    g_Config.Width = std::max<float>(allOptionGroupNamesWidth, biggestOptionGroupWidth);

    // Make the menu tall enough for the biggest option group
    g_Config.Height = biggestOptionGroupHeight;

    // Move the menu to right side of the screen (double cast to round to closest integer value)
    g_Config.X = static_cast<float>(static_cast<uint32_t>(UI::DisplayWidth - g_Config.Width - 10.0f));
}

void Menu::RenderControlsTexts()
{
    float yOffset = 10.0f;
    float fontScale = 0.8f;
    float padding = g_Config.Padding * fontScale;
    float borderWidth = g_Config.BorderWidth * fontScale;

    UI::TextProps props = {};
    props.X = 10.0f;
    props.FontScale = fontScale;
    props.Color = g_Config.TextColor;
    props.BackgroundColor = g_Config.BackgroundColor;
    props.BorderWidth = borderWidth;
    props.BorderColor = g_Config.Color;
    props.BorderPosition = UI::Border_All;

    props.Y = yOffset;
    props.Text = "Hold " CHAR_LT " & press " CHAR_LEFT " to " + std::string(!m_Open ? "Open." : "Close.");
    yOffset += UI::GetTextHeight(props.Text, fontScale) + padding * 3 + borderWidth * 2;
    UI::DrawText(props);

    props.Y = yOffset;
    props.Text = "Use " CHAR_UP CHAR_DOWN " to scroll, " CHAR_X " to select, " CHAR_RS " to go back.";
    yOffset += UI::GetTextHeight(props.Text, fontScale) + padding * 3 + borderWidth * 2;
    UI::DrawText(props);

    props.Y = yOffset;
    props.Text = "Use " CHAR_LB " & " CHAR_RB " to switch menus.";
    yOffset += UI::GetTextHeight(props.Text, fontScale) + padding * 3 + borderWidth * 2;
    UI::DrawText(props);
}

bool Menu::ToggleDebugBuilds(void *pParameters)
{
    XASSERT(pParameters != nullptr);

    bool enabled = *reinterpret_cast<bool *>(pParameters);

    if (!enabled)
    {
        DebugEnabler::Disable();
        return true;
    }

    HRESULT hr = DebugEnabler::Enable();

    return SUCCEEDED(hr);
}

bool Menu::SaveSettings(void *)
{
    HRESULT hr = g_pPlugin->SaveConfig();
    bool success = SUCCEEDED(hr);

    Xam::XNotify(
        success ? "Settings Saved" : "Could not save settings",
        success ? Xam::XNOTIFYUI_TYPE_PREFERRED_REVIEW : Xam::XNOTIFYUI_TYPE_AVOID_REVIEW
    );

    return success;
}

bool Menu::ResetSettings(void *)
{
    g_Config.Reset();

    return true;
}
