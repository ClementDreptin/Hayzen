#include "pch.h"
#include "Options/SelectOption.h"

#include "Core/Settings.h"
#include "Core/UI.h"

SelectOption::SelectOption()
    : Option(), m_CurrentOptionIndex(0)
{
}

SelectOption::SelectOption(const std::string &name, const std::vector<std::string> &options)
    : Option(name, nullptr), m_Options(options), m_CurrentOptionIndex(0)
{
}

SelectOption::SelectOption(const std::string &name, const std::vector<std::string> &options, Callback callback)
    : Option(name, callback), m_Options(options), m_CurrentOptionIndex(0)
{
}

bool SelectOption::Update(Input::Gamepad *pGamepad)
{
    XASSERT(pGamepad != nullptr);

    // Allow the user to change the option index with DPAD LEFT/DPAD RIGHT
    if (pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
    {
        // Loop around the options
        size_t newOptionIndex = m_CurrentOptionIndex;
        if (m_CurrentOptionIndex == 0)
            newOptionIndex = m_Options.size() - 1;
        else
            newOptionIndex--;

        // If there is a callback, only update the option index if the call succeeds
        if (m_Callback != nullptr)
        {
            bool success = m_Callback(&newOptionIndex);
            if (success)
                m_CurrentOptionIndex = newOptionIndex;
        }
        else
            m_CurrentOptionIndex = newOptionIndex;
    }
    else if (pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
    {
        // Loop around the options
        size_t newOptionIndex = m_CurrentOptionIndex;
        if (m_CurrentOptionIndex == m_Options.size() - 1)
            newOptionIndex = 0;
        else
            newOptionIndex++;

        // If there is a callback, only update the option index if the call succeeds
        if (m_Callback != nullptr)
        {
            bool success = m_Callback(&newOptionIndex);
            if (success)
                m_CurrentOptionIndex = newOptionIndex;
        }
        else
            m_CurrentOptionIndex = newOptionIndex;
    }

    return false;
}

void SelectOption::Render(float x, float y, float width)
{
    // Call the parent to render the option name
    Option::Render(x, y, width);

    XASSERT(m_CurrentOptionIndex < m_Options.size());

    const std::string &text = m_Options[m_CurrentOptionIndex];

    UI::TextProps props = {};
    props.X = x + width - UI::GetTextWidth(text) - Settings::Padding;
    props.Y = y;
    props.Text = text;
    props.Color = Settings::TextColor;

    UI::DrawText(props);
}

float SelectOption::GetMinWidth() const
{
    // Return the cached value if the minimum width has already been calculated
    if (m_CachedMinWidth != 0.0f)
        return m_CachedMinWidth;

    // Find the longest text
    float longestTextWidth = 0.0f;
    for (size_t i = 0; i < m_Options.size(); i++)
    {
        float textWidth = UI::GetTextWidth(m_Options[i]);
        if (longestTextWidth < textWidth)
            longestTextWidth = textWidth;
    }

    // The option layout is as follow
    // padding | optionName | padding | padding | text | padding
    m_CachedMinWidth = UI::GetTextWidth(m_Name) + longestTextWidth + Settings::Padding * 4;

    return m_CachedMinWidth;
}
