#pragma once

#include "Options/Option.h"

class ToggleOption : public Option
{
public:
    ToggleOption();

    ToggleOption(const std::string &name, const ValueOrPtr<bool> &active);

    ToggleOption(const std::string &name, Callback callback, const ValueOrPtr<bool> &active);

    virtual bool Update(Input::Gamepad *pGamepad) override;

    virtual void Render(float x, float y, float width) override;

private:
    ValueOrPtr<bool> m_Active;
};
