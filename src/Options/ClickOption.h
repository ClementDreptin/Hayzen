#pragma once

#include "Options/Option.h"

class ClickOption : public Option
{
public:
    ClickOption();

    ClickOption(const std::string &name, Callback callback, void *pParameters = nullptr);

    virtual bool Update(Input::Gamepad *pGamepad) override;

    virtual void Render(float x, float y, float width) override;

private:
    void *m_pParameters;
};
