#pragma once

#include "Options/OptionGroup.h"

class Menu
{
public:
    Menu();

    Menu(const std::vector<OptionGroup> &optionGroups);

    inline bool Open() const { return m_Open; }

    inline void Open(bool open) { m_Open = open; }

    void Update(Input::Gamepad *pGamepad);

    void Render();

private:
    bool m_Open;
    std::vector<OptionGroup> m_OptionGroups;
    size_t m_CurrentOptionGroupIndex;

    mutable float m_CachedOptionGroupHeadersHeight;

    void AddSettingsGroup();

    float GetOptionGroupHeadersHeight() const;

    void RenderOptionGroupHeaders();

    void CalculateMenuDimensions();

    void RenderControlsTexts();

    bool ToggleDebugBuilds(bool enabled);

    bool SaveSettings();

    bool ResetSettings();
};
