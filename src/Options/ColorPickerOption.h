#pragma once

#include "Options/SubOptionGroup.h"

class ColorPickerOption : public SubOptionGroup
{
public:
    ColorPickerOption();

    ColorPickerOption(const std::string &name, Callback callback, const ValueOrPtr<D3DCOLOR> &color);

    virtual bool Update(Input::Gamepad *pGamepad) override;

private:
    ValueOrPtr<D3DCOLOR> m_Color;
    uint32_t m_Red;
    uint32_t m_Green;
    uint32_t m_Blue;
    uint32_t m_Alpha;
};
