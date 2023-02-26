#include "pch.h"
#include "Options/ClickOption.h"

ClickOption::ClickOption()
    : Option()
{
}

ClickOption::ClickOption(const std::string &name, Callback callback)
    : Option(name, callback)
{
}

bool ClickOption::Update(Input::Gamepad *pGamepad)
{
    // Allow the user to click on the option
    if (pGamepad->PressedButtons & XINPUT_GAMEPAD_X)
        m_Callback(nullptr);

    return false;
}

void ClickOption::Render(float x, float y, float width)
{
    // Call the parent to render the option name
    Option::Render(x, y, width);
}
