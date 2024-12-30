#include "pch.h"
#include "Options/Option.h"

#include "Core/Config.h"
#include "Core/UI.h"

Option::Option()
    : m_Callback(nullptr), m_IsSelected(false), m_CachedMinWidth(0.0f), m_CachedMinHeight(0.0f)
{
}

Option::Option(const std::string &name, Callback callback)
    : m_Name(name), m_Callback(callback), m_CachedMinWidth(0.0f), m_CachedMinHeight(0.0f)
{
}

void Option::Render(float x, float y, float width)
{
    // Render the background if the option is selected
    if (m_IsSelected)
    {
        UI::RectangleProps props = {};
        props.X = x + g_Config.Gap;
        props.Y = y + g_Config.Gap;
        props.Width = width - g_Config.Gap * 2;
        props.Height = GetMinHeight() - g_Config.Gap * 2;
        props.Color = g_Config.Color;

        UI::DrawRectangle(props);
    }

    // Render the text
    UI::TextProps props = {};
    props.X = x + g_Config.Padding;
    props.Y = y;
    props.Text = m_Name;
    props.Color = g_Config.TextColor;

    UI::DrawText(props);
}

float Option::GetMinWidth() const
{
    // Return the cached value if the minimum width has already been calculated
    if (m_CachedMinWidth != 0.0f)
        return m_CachedMinWidth;

    m_CachedMinWidth = UI::GetTextWidth(m_Name) + g_Config.Padding * 2;

    // Take into account some space between the option name and the potential text on the right (e.g. the number for RangeOption)
    m_CachedMinWidth += 100.0f;

    return m_CachedMinWidth;
}

float Option::GetMinHeight() const
{
    // Return the cached value if the minimum height has already been calculated
    if (m_CachedMinHeight != 0.0f)
        return m_CachedMinHeight;

    m_CachedMinHeight = UI::GetTextHeight(m_Name) + g_Config.Padding * 2;

    return m_CachedMinHeight;
}
