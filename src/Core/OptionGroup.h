#pragma once

#include "Core/Input.h"
#include "Options/Option.h"
#include "UI/Rectangle.h"

class OptionGroup
{
public:
    OptionGroup();

    OptionGroup(const std::string &name, const std::vector<std::shared_ptr<Option>> &options);

    inline const std::string &GetName() const { return m_Name; }

    inline const std::vector<std::shared_ptr<Option>> &GetOptions() const { return m_Options; }

    void Update(Input::Gamepad *pGamepad);

    void Render(float x, float y, float width = 0.0f, float height = 0.0f);

    float GetMinWidth();

    float GetMinHeight();

private:
    std::string m_Name;
    std::vector<std::shared_ptr<Option>> m_Options;

    size_t m_CurrentSelectedOptionIndex;

    float m_CachedMinWidth;
    float m_CachedMinHeight;

    Rectangle m_Background;

    void RenderBackground(float x, float y, float width, float height);
};
