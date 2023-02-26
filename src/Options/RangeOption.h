#pragma once

#include "Options/Option.h"
#include "UI/Text.h"

template <typename T>
class RangeOption : public Option
{
public:
    RangeOption();

    RangeOption(const std::string &name, Callback callback, const ValueOrPtr<T> &value, T min, T max, T step);

    virtual bool Update(Input::Gamepad *pGamepad) override;

    virtual void Render(float x, float y, float width) override;

private:
    T m_Min;
    T m_Max;
    T m_Step;
    ValueOrPtr<T> m_Current;

    Text m_Text;
};
