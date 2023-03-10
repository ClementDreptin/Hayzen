#include "pch.h"
#include "Options/SubOptionGroup.h"

#include "UI/Renderer.h"

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
    // Allow the user to open/close the sub option group with X/RS
    if (pGamepad->PressedButtons & XINPUT_GAMEPAD_X)
        m_Open = true;
    else if (pGamepad->PressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
        m_Open = false;

    // Update the sub option group if it's open
    if (m_Open)
        m_OptionGroup.Update(pGamepad);

    return m_Open;
}

void SubOptionGroup::Render(float x, float y, float width)
{
    // Call the parent to render the option name
    Option::Render(x, y, width);

    // Render the option group if it's open
    if (m_Open)
    {
        // Render the sub option group on the left or the right of the menu, depending on where there's space
        float subOptionGroupWidth = m_OptionGroup.GetMinWidth() + Layout::BorderWidth * 2;
        float currentOptionWidth = width + Layout::BorderWidth * 2;
        float leftX = x - subOptionGroupWidth;
        float rightX = x + currentOptionWidth;

        float finalX = rightX;
        if (leftX < 0.0f)
            finalX = rightX;
        else if (rightX + subOptionGroupWidth > Renderer::DisplayWidth)
            finalX = leftX;

        m_OptionGroup.Render(finalX, y);
    }
}
