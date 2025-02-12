#pragma once

#include "Options/OptionGroup.h"

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

    mutable float m_CachedOptionGroupHeadersHeight;

    void AddSettingsGroup();

    float GetOptionGroupHeadersHeight() const;

    void RenderOptionGroupHeaders();

    void CalculateMenuDimensions();

    bool ToggleDebugBuilds(void *);

    bool SaveSettings(void *);

    bool ResetSettings(void *);
};
