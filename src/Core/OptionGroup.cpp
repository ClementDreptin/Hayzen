#include "pch.h"
#include "Core/OptionGroup.h"

#include "UI/Layout.h"

OptionGroup::OptionGroup()
    : m_CurrentSelectedOptionIndex(0), m_CachedMinWidth(0.0f), m_CachedMinHeight(0.0f)
{
}

OptionGroup::OptionGroup(const std::vector<std::shared_ptr<Option>> &options)
    : m_Name(""), m_Options(options), m_CurrentSelectedOptionIndex(0), m_CachedMinWidth(0.0f), m_CachedMinHeight(0.0f)
{
}

OptionGroup::OptionGroup(const std::string &name, const std::vector<std::shared_ptr<Option>> &options)
    : m_Name(name), m_Options(options), m_CurrentSelectedOptionIndex(0), m_CachedMinWidth(0.0f), m_CachedMinHeight(0.0f)
{
}

void OptionGroup::Update(Input::Gamepad *pGamepad)
{
    // Update the currently selected option and return if the option is blocking (meaning it opened a sub option group
    // and wants to prevent its parent option group from updating)
    bool blocking = m_Options[m_CurrentSelectedOptionIndex]->Update(pGamepad);
    if (blocking)
        return;

    // Allow the user to select options with the DPAD
    if (pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_UP)
    {
        // If the scroller is already at the top, send it to the bottom
        if (m_CurrentSelectedOptionIndex == 0)
            m_CurrentSelectedOptionIndex = m_Options.size() - 1;
        else
            m_CurrentSelectedOptionIndex--;
    }
    else if (pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
    {
        // If the scroller is already at the bottom, send it to the top
        if (m_CurrentSelectedOptionIndex == m_Options.size() - 1)
            m_CurrentSelectedOptionIndex = 0;
        else
            m_CurrentSelectedOptionIndex++;
    }

    // Change the option states according to the currently selected option
    for (size_t i = 0; i < m_Options.size(); i++)
        m_Options[i]->Select(i == m_CurrentSelectedOptionIndex);
}

void OptionGroup::Render(float x, float y, float width, float height)
{
    float widthToUse = width != 0.0f ? width : GetMinWidth();
    float heightToUse = height != 0.0f ? height : GetMinHeight();

    // Render the background
    RenderBackground(x, y, widthToUse, heightToUse);

    // Render the options
    for (size_t i = 0; i < m_Options.size(); i++)
        m_Options[i]->Render(x, y + i * Layout::LineHeight, widthToUse);
}

float OptionGroup::GetMinWidth()
{
    // Return the cached value if the minimum width has already been calculated
    if (m_CachedMinWidth != 0.0f)
        return m_CachedMinWidth;

    // Find the longest option name
    for (size_t i = 0; i < m_Options.size(); i++)
    {
        float optionNameWidth = m_Options[i]->GetMinWidth();
        if (m_CachedMinWidth < optionNameWidth)
            m_CachedMinWidth = optionNameWidth;
    }

    return m_CachedMinWidth;
}

float OptionGroup::GetMinHeight()
{
    // Return the cached value if the minimum width has already been calculated
    if (m_CachedMinHeight != 0.0f)
        return m_CachedMinHeight;

    m_CachedMinHeight = m_Options.size() * Layout::LineHeight;

    return m_CachedMinHeight;
}

void OptionGroup::RenderBackground(float x, float y, float width, float height)
{
    Rectangle::Props props = { 0 };
    props.X = x;
    props.Y = y;
    props.Width = width;
    props.Height = height;
    props.Color = Layout::BackgroundColor;
    props.BorderWidth = Layout::BorderWidth;
    props.BorderColor = Layout::Color;
    props.BorderPosition = Border::Border_All;

    m_Background.Render(props);
}
