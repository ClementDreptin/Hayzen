#pragma once

#include "Options/Option.h"
#include "UI/Rectangle.h"

// Disable the C4481 warning for the override keyword
#pragma warning(push)
#pragma warning(disable : 4481)

class ToggleOption : public Option
{
public:
    ToggleOption();

    ToggleOption(const std::string &name, Callback callback, const ValueOrPtr<bool> &active = false);

    virtual bool Update(Input::Gamepad *pGamepad) override;

    virtual void Render(float x, float y, float width) override;

private:
    ValueOrPtr<bool> m_Active;

    Rectangle m_RadioBox;
};

#pragma warning(pop)
