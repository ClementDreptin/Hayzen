#include "pch.h"
#include "Options/ToggleOption.h"

#include "UI/Layout.h"

ToggleOption::ToggleOption()
    : Option(), m_Active(false)
{
}

ToggleOption::ToggleOption(const std::string &name, Callback callback, const ValueOrPtr<bool> &active)
    : Option(name, callback), m_Active(active)
{
}

bool ToggleOption::Update(Input::Gamepad *pGamepad)
{
    // Allow the user to toggle the option
    if (pGamepad->PressedButtons & XINPUT_GAMEPAD_X)
    {
        bool newValue = !*m_Active;
        if (m_Callback != nullptr)
        {
            bool success = m_Callback(&newValue);
            if (success)
                m_Active = newValue;
        }
        else
            m_Active = newValue;
    }

    return false;
}

void ToggleOption::Render(float x, float y, float width)
{
    // Call the parent to render the option name
    Option::Render(x, y, width);

    // Render the radio box
    float radioBoxSize = Layout::LineHeight * 0.5f;
    Rectangle::Props props = { 0 };
    props.X = x + width - radioBoxSize - Layout::Padding;
    props.Y = y + Layout::LineHeight / 2 - radioBoxSize / 2;
    props.Width = radioBoxSize;
    props.Height = radioBoxSize;
    props.Color = *m_Active ? Layout::Color : D3DCOLOR_XRGB(0, 0, 0);
    props.BorderWidth = 1.0f;
    props.BorderColor = D3DCOLOR_XRGB(255, 255, 255);
    props.BorderPosition = Border::Border_All;

    m_RadioBox.Render(props);
}
