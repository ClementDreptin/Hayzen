#include "pch.h"
#include "Options/RangeOption.h"

#include "UI/Renderer.h"
#include "UI/Layout.h"

template <typename T>
RangeOption<T>::RangeOption()
    : Option(), m_Min(static_cast<T>(0)), m_Max(static_cast<T>(0)), m_Current(static_cast<T>(0))
{
}

template<typename T>
RangeOption<T>::RangeOption(const std::string &name, Callback callback, const ValueOrPtr<T> &value, T min, T max, T step)
    : Option(name, callback), m_Min(min), m_Max(max), m_Step(step), m_Current(value)
{
}

template<typename T>
bool RangeOption<T>::Update(Input::Gamepad *pGamepad)
{
    // Allow the user to change the value with DPAD LEFT/DPAD RIGHT
    if (pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
    {
        if (*m_Current < m_Max)
        {
            T newValue = m_Current + m_Step;
            if (m_Callback != nullptr)
            {
                bool success = m_Callback(&newValue);
                if (success)
                    m_Current = newValue;
            }
            else
                m_Current = newValue;
        }
    }
    else if (pGamepad->PressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
    {
        if (*m_Current > m_Min)
        {
            T newValue = m_Current - m_Step;
            if (m_Callback != nullptr)
            {
                bool success = m_Callback(&newValue);
                if (success)
                    m_Current = newValue;
            }
            else
                m_Current = newValue;
        }
    }

    return false;
}

template<typename T>
void RangeOption<T>::Render(float x, float y, float width)
{
    using namespace Renderer;

    // Call the parent to render the option name
    Option::Render(x, y, width);

    // Create a wide string from the current number value
    std::string text = std::to_string(static_cast<long double>(*m_Current));

    // Render the text with the number
    Text::Props props = { 0 };
    props.X = x + width - GetTextWidth(text) - Layout::Padding;
    props.Y = y + Layout::Padding;
    props.Text = text;
    props.Color = Layout::TextColor;
    m_Text.Render(props);
}

template class RangeOption<float>;
template class RangeOption<uint32_t>;
