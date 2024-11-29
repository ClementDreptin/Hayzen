#include "pch.h"
#include "Core/Menu.h"

#include "Core/Plugin.h"
#include "Core/Settings.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "Options/SubOptionGroup.h"
#include "Options/ColorPickerOption.h"
#include "UI/Renderer.h"
#include "UI/UI.h"

Menu::Menu()
    : m_CurrentOptionGroupIndex(0), m_CachedOptionGroupHeadersHeight(0.0f)
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
    RenderOptionGroupHeaders();

    float optionGroupHeadersHeight = GetOptionGroupHeadersHeight();
    m_OptionGroups[m_CurrentOptionGroupIndex].Render(Settings::X, Settings::Y + optionGroupHeadersHeight, Settings::Width, Settings::Height);
}

void Menu::AddSettingsGroup()
{
    std::vector<std::shared_ptr<Option>> options;

    options.emplace_back(MakeOption(ToggleOption, "Show Controls", &Settings::DisplayControlsTexts));

    if (!Xam::IsDevkit())
        options.emplace_back(MakeOption(ToggleOption, "Allow Debug Builds", std::bind(&Menu::ToggleDebugBuilds, this, std::placeholders::_1), &Settings::AllowDebugBuilds));

    std::vector<std::shared_ptr<Option>> menuPositionOptions;
    menuPositionOptions.emplace_back(MakeOption(RangeOption<float>, "X", &Settings::X, Settings::BorderWidth, Renderer::DisplayWidth, 10.0f));
    menuPositionOptions.emplace_back(MakeOption(RangeOption<float>, "Y", &Settings::Y, Settings::BorderWidth, Renderer::DisplayHeight, 10.0f));
    options.emplace_back(MakeOption(SubOptionGroup, "Menu Position", menuPositionOptions));

    options.emplace_back(MakeOption(ColorPickerOption, "Menu Color", &Settings::Color));
    options.emplace_back(MakeOption(ClickOption, "Save Settings", std::bind(&Menu::SaveSettings, this, std::placeholders::_1)));
    options.emplace_back(MakeOption(ClickOption, "Reset Settings", std::bind(&Menu::ResetSettings, this, std::placeholders::_1)));
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
        float currentOptionGroupHeaderHeight = Renderer::GetTextHeight(m_OptionGroups[i].GetName()) + Settings::Padding * 2;
        if (currentOptionGroupHeaderHeight > m_CachedOptionGroupHeadersHeight)
            m_CachedOptionGroupHeadersHeight = currentOptionGroupHeaderHeight;
    }
    m_CachedOptionGroupHeadersHeight += Settings::BorderWidth;

    return m_CachedOptionGroupHeadersHeight;
}

void Menu::RenderOptionGroupHeaders()
{
    float optionGroupHeadersHeight = GetOptionGroupHeadersHeight();

    for (size_t i = 0; i < m_OptionGroups.size(); i++)
    {
        // The X offset is the sum of the previous option group header widths
        float offset = Settings::X;
        for (size_t j = 0; j < i; j++)
            offset += (Renderer::GetTextWidth(m_OptionGroups[j].GetName()) + Settings::Padding * 2 + Settings::BorderWidth);

        UI::TextProps props = {};
        props.X = offset;
        props.Y = Settings::Y;
        props.Text = m_OptionGroups[i].GetName();
        props.BackgroundHeight = optionGroupHeadersHeight;
        props.BorderWidth = Settings::BorderWidth;

        // Make the header more transparent when the option group header is not selected
        uint8_t alpha = 100;
        props.Color = i == m_CurrentOptionGroupIndex ? Settings::TextColor : D3DCOLOR_ALPHA(Settings::TextColor, alpha);
        props.BackgroundColor = i == m_CurrentOptionGroupIndex ? Settings::BackgroundColor : D3DCOLOR_ALPHA(Settings::BackgroundColor, alpha);
        props.BorderColor = i == m_CurrentOptionGroupIndex ? Settings::Color : D3DCOLOR_ALPHA(Settings::Color, alpha);

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
        allOptionGroupNamesWidth += (Renderer::GetTextWidth(m_OptionGroups[i].GetName()) + Settings::Padding * 2);

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
    allOptionGroupNamesWidth += Settings::BorderWidth * (m_OptionGroups.size() - 1);

    // Make the menu width the max between all the option names width and the longest option name
    Settings::Width = std::max<float>(allOptionGroupNamesWidth, biggestOptionGroupWidth);

    // Make the menu tall enough for the biggest option group
    Settings::Height = biggestOptionGroupHeight;

    // Move the menu to right side of the screen (double cast to round to closest integer value)
    Settings::X = static_cast<float>(static_cast<uint32_t>(Renderer::DisplayWidth - Settings::Width - 10.0f));
}

bool Menu::ToggleDebugBuilds(void *pParameters)
{
    // Setting Settings::AllowDebugBuilds is normally done based of the return value
    // of this function but we need to save the settings to disk so we have to set
    // Settings::AllowDebugBuilds before the function ends
    bool enabled = *reinterpret_cast<bool *>(pParameters);
    Settings::AllowDebugBuilds = enabled;

    bool couldSave = SaveSettings(nullptr);
    if (!couldSave)
        return false;

    Xam::XNotify("Reboot to apply changes");

    return true;
}

bool Menu::SaveSettings(void *)
{
    bool result = g_pPlugin->GetConfig().Save();

    Xam::XNotify(
        result ? "Settings Saved" : "Could not save settings",
        result ? XNOTIFYUI_TYPE_PREFERRED_REVIEW : XNOTIFYUI_TYPE_AVOID_REVIEW
    );

    return result;
}

bool Menu::ResetSettings(void *)
{
    Settings::Reset();

    return true;
}
