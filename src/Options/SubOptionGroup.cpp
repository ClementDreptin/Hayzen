#include "pch.h"
#include "Options/SubOptionGroup.h"

#include "Core/Settings.h"
#include "UI/UI.h"

SubOptionGroup::SubOptionGroup()
    : Option(), m_Open(false)
{
}

SubOptionGroup::SubOptionGroup(const std::string &name, const std::vector<std::shared_ptr<Option>> &options)
    : Option(name, nullptr), m_Open(false), m_OptionGroup(OptionGroup(options))
{
}

bool SubOptionGroup::Update(Input::Gamepad *pGamepad)
{
    // Allow the user to close the sub option group with RS
    if (pGamepad->PressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
        m_Open = false;

    if (m_Open)
        m_OptionGroup.Update(pGamepad);

    // Allow the user to open the sub option group with X
    // Opening the option group is done after updating it so that its first option doesn't
    // get a gamepad state with X pressed. If opening was done before, the first option of
    // the option group would be click right after the option group is open.
    if (pGamepad->PressedButtons & XINPUT_GAMEPAD_X)
        m_Open = true;

    return m_Open;
}

void SubOptionGroup::Render(float x, float y, float width)
{
    // Call the parent to render the option name
    Option::Render(x, y, width);

    // Render a ">" on the right to indicate that there's a sub menu
    std::string text = ">";
    UI::TextProps props = {};
    props.X = x + width - UI::GetTextWidth(text) - Settings::Padding;
    props.Y = y - 4.0f; // Try to center the ">" vertically
    props.Text = text;
    props.Color = Settings::TextColor;
    UI::DrawText(props);

    // If the option group is not open, don't go further
    if (!m_Open)
        return;

    // Render the sub option group on the left or the right of the menu, depending on where there's space
    float subOptionGroupWidth = m_OptionGroup.GetMinWidth() + Settings::BorderWidth * 2;
    float currentOptionWidth = width + Settings::BorderWidth * 2;
    float leftX = x - subOptionGroupWidth;
    float rightX = x + currentOptionWidth;

    float finalX = rightX;
    if (leftX < 0.0f)
        finalX = rightX;
    else if (rightX + subOptionGroupWidth > UI::DisplayWidth)
        finalX = leftX;

    m_OptionGroup.Render(finalX, y);
}
