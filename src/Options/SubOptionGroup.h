#pragma once

#include "Core/OptionGroup.h"

// Disable the C4481 warning for the override keyword
#pragma warning(push)
#pragma warning(disable : 4481)

class SubOptionGroup : public Option
{
public:
    SubOptionGroup();

    SubOptionGroup(const std::string &name, Callback callback, const OptionGroup &optionGroup);

    virtual bool Update(Input::Gamepad *pGamepad) override;

    virtual void Render(float x, float y, float width) override;

protected:
    bool m_Open;
    OptionGroup m_OptionGroup;
};

#pragma warning(pop)
