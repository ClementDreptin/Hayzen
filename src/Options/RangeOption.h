#pragma once

#include "Options/Option.h"

template<typename T>
class RangeOption : public Option
{
public:
    typedef std::function<bool(T)> Callback;

    RangeOption();

    RangeOption(const std::string &name, const ValueOrPtr<T> &value, T min, T max, T step);

    RangeOption(const std::string &name, Callback callback, const ValueOrPtr<T> &value, T min, T max, T step);

    virtual bool Update(Input::Gamepad *pGamepad) override;

    virtual void Render(float x, float y, float width) override;

private:
    Callback m_Callback;
    T m_Min;
    T m_Max;
    T m_Step;
    ValueOrPtr<T> m_Current;
};
