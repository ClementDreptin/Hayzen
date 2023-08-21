#include "pch.h"
#include "Core/Menu.h"

#include "Core/Plugin.h"
#include "Core/Context.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "Options/SubOptionGroup.h"
#include "Options/ColorPickerOption.h"
#include "UI/Renderer.h"
#include "UI/Layout.h"

Menu::Menu()
    : m_CurrentOptionGroupIndex(0), m_Config("hdd:\\Hayzen.ini"), m_CachedOptionGroupHeadersHeight(0.0f)
{
    CreateConfig();
}

void Menu::Init(const std::vector<OptionGroup> &optionGroups)
{
    m_OptionGroups = optionGroups;

    AddCustomizationGroup();

    CalculateMenuDimensions();

    // Load the menu settings from the config (doesn't do anything if the config file doesn't exist)
    m_Config.Load();

    m_OptionGroupHeaders = std::vector<Text>(m_OptionGroups.size(), Text());
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
    m_OptionGroups[m_CurrentOptionGroupIndex].Render(Layout::X, Layout::Y + optionGroupHeadersHeight, Layout::Width, Layout::Height);
}

void Menu::AddCustomizationGroup()
{
    std::vector<std::shared_ptr<Option>> options;

    options.emplace_back(MakeOption(ToggleOption, "Show Controls", &Context::DisplayControlsTexts));

    std::vector<std::shared_ptr<Option>> menuPositionOptions;
    menuPositionOptions.emplace_back(MakeOption(RangeOption<float>, "X", &Layout::X, Layout::BorderWidth, Renderer::DisplayWidth, 10.0f));
    menuPositionOptions.emplace_back(MakeOption(RangeOption<float>, "Y", &Layout::Y, Layout::BorderWidth, Renderer::DisplayHeight, 10.0f));
    options.emplace_back(MakeOption(SubOptionGroup, "Menu Position", menuPositionOptions));

    options.emplace_back(MakeOption(ColorPickerOption, "Menu Color", &Layout::Color));
    options.emplace_back(MakeOption(ClickOption, "Save Settings", std::bind(&Menu::SaveSettings, this, std::placeholders::_1)));
    options.emplace_back(MakeOption(ClickOption, "Reset Settings", std::bind(&Menu::ResetSettings, this, std::placeholders::_1)));
    m_OptionGroups.emplace_back(OptionGroup("Customization", options));
}

float Menu::GetOptionGroupHeadersHeight() const
{
    // Return the cached value if the minimum width has already been calculated
    if (m_CachedOptionGroupHeadersHeight != 0.0f)
        return m_CachedOptionGroupHeadersHeight;

    float m_CachedOptionGroupHeadersHeight = 0.0f;
    for (size_t i = 0; i < m_OptionGroupHeaders.size(); i++)
    {
        float currentOptionGroupHeaderHeight = Renderer::GetTextHeight(m_OptionGroups[i].GetName()) + Layout::Padding * 2;
        if (currentOptionGroupHeaderHeight > m_CachedOptionGroupHeadersHeight)
            m_CachedOptionGroupHeadersHeight = currentOptionGroupHeaderHeight;
    }
    m_CachedOptionGroupHeadersHeight += Layout::BorderWidth;

    return m_CachedOptionGroupHeadersHeight;
}

void Menu::RenderOptionGroupHeaders()
{
    float optionGroupHeadersHeight = GetOptionGroupHeadersHeight();

    for (size_t i = 0; i < m_OptionGroupHeaders.size(); i++)
    {
        // The X offset is the sum of the previous option group header widths
        float offset = Layout::X;
        for (size_t j = 0; j < i; j++)
            offset += (Renderer::GetTextWidth(m_OptionGroups[j].GetName()) + Layout::Padding * 2 + Layout::BorderWidth);

        Text::Props props = { 0 };
        props.X = offset;
        props.Y = Layout::Y;
        props.Text = m_OptionGroups[i].GetName();
        props.BackgroundHeight = optionGroupHeadersHeight;
        props.BorderWidth = Layout::BorderWidth;

        // Make the header more transparent when the option group header is not selected
        uint8_t alpha = 100;
        props.Color = i == m_CurrentOptionGroupIndex ? Layout::TextColor : D3DCOLOR_ALPHA(Layout::TextColor, alpha);
        props.BackgroundColor = i == m_CurrentOptionGroupIndex ? Layout::BackgroundColor : D3DCOLOR_ALPHA(Layout::BackgroundColor, alpha);
        props.BorderColor = i == m_CurrentOptionGroupIndex ? Layout::Color : D3DCOLOR_ALPHA(Layout::Color, alpha);

        // m_OptionGroups[i] is selected
        if (i == m_CurrentOptionGroupIndex)
            props.BorderPosition = static_cast<Border::Position>(Border::Border_Left | Border::Border_Top | Border::Border_Right);

        // m_OptionGroups[i] is to the left of the selected option group
        else if (i < m_CurrentOptionGroupIndex)
            props.BorderPosition = static_cast<Border::Position>(Border::Border_Left | Border::Border_Top);

        // m_OptionGroups[i] is to the right of the selected option group
        else if (i > m_CurrentOptionGroupIndex)
            props.BorderPosition = static_cast<Border::Position>(Border::Border_Right | Border::Border_Top);

        m_OptionGroupHeaders[i].Render(props);
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
        allOptionGroupNamesWidth += (Renderer::GetTextWidth(m_OptionGroups[i].GetName()) + Layout::Padding * 2);

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
    allOptionGroupNamesWidth += Layout::BorderWidth * (m_OptionGroups.size() - 1);

    // Make the menu width the max between all the option names width and the longest option name
    Layout::Width = std::max<float>(allOptionGroupNamesWidth, biggestOptionGroupWidth);

    // Make the menu tall enough for the biggest option group
    Layout::Height = biggestOptionGroupHeight;

    // Move the menu to right side of the screen (double cast to round to closest integer value)
    Layout::X = static_cast<float>(static_cast<uint32_t>(Renderer::DisplayWidth - Layout::Width - 10.0f));
}

void Menu::CreateConfig()
{
    // If the plugin path was not found (this might happen if the plugin is loaded through something else than DashLaunch)
    // just keep the default config path used in the constructor initializer list
    std::string pluginPath = g_pPlugin->GetPath();
    if (pluginPath.empty())
        return;

    // Extract the directory from the plugin path
    char pluginDirectory[MAX_PATH] = { 0 };
    _splitpath_s(
        pluginPath.c_str(),
        nullptr, 0,
        pluginDirectory, sizeof(pluginDirectory),
        nullptr, 0,
        nullptr, 0
    );

    // Rebuild the config file path from the plugin directory
    std::stringstream configFilePath;
    configFilePath << "hdd:";
    configFilePath << pluginDirectory;
    configFilePath << "Hayzen.ini";

    m_Config = Config(configFilePath.str());
}

bool Menu::SaveSettings(void *)
{
    bool result = m_Config.Save();

    Xam::XNotify(
        result ? "Settings Saved" : "Could not save settings",
        result ? XNOTIFYUI_TYPE_PREFERRED_REVIEW : XNOTIFYUI_TYPE_AVOID_REVIEW
    );

    return result;
}

bool Menu::ResetSettings(void *)
{
    Layout::Reset();
    Context::DisplayControlsTexts = true;

    return true;
}
