#include "pch.h"
#include "Options/ColorPickerOption.h"

#include "Options/RangeOption.h"

ColorPickerOption::ColorPickerOption()
    : SubOptionGroup(), m_Color(D3DCOLOR(0)), m_Red(0), m_Green(0), m_Blue(0), m_Alpha(0)
{
}

ColorPickerOption::ColorPickerOption(const std::string &name, const ValueOrPtr<D3DCOLOR> &color)
    : SubOptionGroup(name, std::vector<std::shared_ptr<Option>>()), m_Color(color), m_Red(D3DCOLOR_GETRED(color)), m_Green(D3DCOLOR_GETGREEN(color)), m_Blue(D3DCOLOR_GETBLUE(color)), m_Alpha(D3DCOLOR_GETALPHA(color))
{
    std::vector<std::shared_ptr<Option>> options;
    options.emplace_back(MakeOption(RangeOption<uint32_t>, "Red", &m_Red, 0, 255, 1));
    options.emplace_back(MakeOption(RangeOption<uint32_t>, "Green", &m_Green, 0, 255, 1));
    options.emplace_back(MakeOption(RangeOption<uint32_t>, "Blue", &m_Blue, 0, 255, 1));
    options.emplace_back(MakeOption(RangeOption<uint32_t>, "Alpha", &m_Alpha, 0, 255, 1));
    m_OptionGroup = OptionGroup(options);
}

bool ColorPickerOption::Update(Input::Gamepad *pGamepad)
{
    XASSERT(pGamepad != nullptr);

    // Call the parent Update method and return early if the sub option group is not open
    bool open = SubOptionGroup::Update(pGamepad);
    if (!open)
        return false;

    // Update the color value when it's changed with DPAD LEFT/DPAD RIGHT
    if (pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_LEFT || pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
        m_Color = D3DCOLOR_RGBA(m_Red, m_Green, m_Blue, m_Alpha);

    // Each channel needs to be set the what is currently in m_Color because it might be a pointer
    // to a color and this color can be changed from outside of the color picker
    m_Red = D3DCOLOR_GETRED(m_Color);
    m_Green = D3DCOLOR_GETGREEN(m_Color);
    m_Blue = D3DCOLOR_GETBLUE(m_Color);
    m_Alpha = D3DCOLOR_GETALPHA(m_Color);

    return true;
}
