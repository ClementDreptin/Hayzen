#pragma once

#include "Options/Option.h"
#include "UI/Text.h"

class SelectOption : public Option
{
public:
    SelectOption();

    SelectOption(const std::string &name, const std::vector<std::string> &options);

    SelectOption(const std::string &name, const std::vector<std::string> &options, Callback callback);

    virtual bool Update(Input::Gamepad *pGamepad) override;

    virtual void Render(float x, float y, float width) override;

    virtual float GetMinWidth() const override;

private:
    std::vector<std::string> m_Options;
    size_t m_CurrentOptionIndex;

    Text m_Text;
};
