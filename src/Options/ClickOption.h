#pragma once

#include "Options/Option.h"

// Disable the C4481 warning for the override keyword
#pragma warning(push)
#pragma warning(disable : 4481)

class ClickOption : public Option
{
public:
    ClickOption();

    ClickOption(const std::string &name, Callback callback);

    virtual bool Update(Input::Gamepad *pGamepad) override;

    virtual void Render(float x, float y, float width) override;
};

#pragma warning(pop)
