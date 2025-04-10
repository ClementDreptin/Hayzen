#pragma once

#include "Options/Option.h"

class OptionGroup
{
public:
    OptionGroup();

    OptionGroup(const std::vector<std::shared_ptr<Option>> &options);

    OptionGroup(const std::string &name, const std::vector<std::shared_ptr<Option>> &options);

    inline const std::string &GetName() const { return m_Name; }

    inline const std::vector<std::shared_ptr<Option>> &GetOptions() const { return m_Options; }

    void Update(Input::Gamepad *pGamepad);

    void Render(float x, float y, float width = 0.0f, float height = 0.0f);

    float GetMinWidth() const;

    float GetMinHeight() const;

private:
    std::string m_Name;
    std::vector<std::shared_ptr<Option>> m_Options;

    size_t m_OptionsToDisplay;
    size_t m_FirstOptionIndex;
    size_t m_LastOptionIndex;
    size_t m_CurrentOptionIndex;

    mutable float m_CachedMinWidth;
    mutable float m_CachedMinHeight;

    void RenderBackground(float x, float y, float width, float height);
};
