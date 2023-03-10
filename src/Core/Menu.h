#pragma once

#include "Core/Input.h"
#include "Core/Config.h"
#include "Core/OptionGroup.h"
#include "UI/Rectangle.h"
#include "UI/Text.h"

class Menu
{
public:
    Menu();

    void Init(const std::vector<OptionGroup> &optionGroups);

    void Update(Input::Gamepad *pGamepad);

    void Render();

private:
    std::vector<OptionGroup> m_OptionGroups;
    size_t m_CurrentOptionGroupIndex;

    std::vector<Text> m_OptionGroupHeaders;

    Config m_Config;

    void AddCustomizationGroup();

    void RenderOptionGroupHeaders();

    void CalculateMenuDimensions();
};
