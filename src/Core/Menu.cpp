#include "pch.h"
#include "Core/Menu.h"

#include "Core/Context.h"
#include "Core/OptionGroup.h"
#include "Options/ClickOption.h"
#include "Options/ToggleOption.h"
#include "Options/RangeOption.h"
#include "Options/ColorPickerOption.h"
#include "UI/Renderer.h"
#include "UI/Layout.h"

using namespace Renderer;

Menu::Menu()
    : m_CurrentOptionGroupIndex(0), m_Config("hdd:\\Hayzen.ini")
{
}

void Menu::Init(const std::vector<OptionGroup> &optionGroups)
{
    m_OptionGroups = optionGroups;

    // Append the customization option group to the existing option groups
    AddCustomizationGroup();

    CalculateMenuDimensions();

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

    // Update the currently selected option group
    m_OptionGroups[m_CurrentOptionGroupIndex].Update(pGamepad);
}

void Menu::Render()
{
    // Render the option group headers
    RenderOptionGroupHeaders();

    // Render the currently selected option group
    float optionGroupHeadersHeight = Layout::LineHeight + Layout::BorderWidth;
    m_OptionGroups[m_CurrentOptionGroupIndex].Render(Layout::X, Layout::Y + optionGroupHeadersHeight, Layout::Width, Layout::Height);
}

static bool SaveSettings(void *pParameters)
{
    Config *pConfig = reinterpret_cast<Config *>(pParameters);

    bool result = pConfig->Save();

    Xam::XNotify(
        result ? "Settings were saved" : "Could not save settings",
        result ? XNOTIFYUI_TYPE_PREFERRED_REVIEW : XNOTIFYUI_TYPE_AVOID_REVIEW
    );

    return result;
}

void Menu::AddCustomizationGroup()
{
    std::vector<std::shared_ptr<Option>> options;
    options.emplace_back(MakeOption(ToggleOption, "Show Controls", nullptr, &Context::DisplayControlsTexts));
    options.emplace_back(MakeOption(RangeOption<float>, "Menu X", nullptr, &Layout::X, Layout::BorderWidth, DisplayWidth, 10.0f));
    options.emplace_back(MakeOption(RangeOption<float>, "Menu Y", nullptr, &Layout::Y, Layout::BorderWidth, DisplayHeight, 10.0f));
    options.emplace_back(MakeOption(ColorPickerOption, "Menu Color", nullptr, &Layout::Color));
    options.emplace_back(MakeOption(ClickOption, "Save Settings", SaveSettings, &m_Config));
    m_OptionGroups.emplace_back(OptionGroup("Customization", options));
}

void Menu::RenderOptionGroupHeaders()
{
    for (size_t i = 0; i < m_OptionGroupHeaders.size(); i++)
    {
        // The X offset is the sum of the previous option group header widths
        float offset = Layout::X;
        for (size_t j = 0; j < i; j++)
            offset += (GetTextWidth(m_OptionGroups[j].GetName()) + Layout::Padding * 2 + Layout::BorderWidth);

        // Create the props
        Text::Props props = { 0 };
        props.X = offset;
        props.Y = Layout::Y;
        props.Text = m_OptionGroups[i].GetName();
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

        // Render m_OptionsGroups[i]
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
        allOptionGroupNamesWidth += (GetTextWidth(m_OptionGroups[i].GetName()) + Layout::Padding * 2);

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

    // Move the menu to right side of the screen (double cast to rounded to closest integer value)
    Layout::X = static_cast<float>(static_cast<uint32_t>(DisplayWidth - Layout::Width - 10.0f));
}
