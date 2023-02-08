#pragma once

#include "Elements/HudElem.h"

// Class to draw a text element, inherits from HudElem.
class Text : public HudElem
{
public:
    // Default constructor.
    Text() {}

    // Constructor.
    Text(const std::string &text, float x, float y, const Color &color, float fontScale = 1.0f);

    // Call R_AddCmdDrawText() of the current game.
    virtual void Draw();

    float GetFontScale() const { return m_FontScale; }

    const std::string &GetText() const { return m_Text; }

    void SetFontScale(float fontScale) { m_FontScale = fontScale; }

    void SetText(const std::string &text) { m_Text = text; }

private:
    float m_FontScale;
    std::string m_Text;
};
