#pragma once

#include "Elements\HudElem.h"


// Class to draw a text element, inherits from HudElem.
class Text : public HudElem
{
public:
    // Default constructor.
    Text() {}

    // Constructor.
    Text(const std::string &strText, float fX, float fY, const Color &color, float fFontScale = 1.0f);

    // Call R_AddCmdDrawText() of the current game.
    virtual void Draw();

    float GetFontScale() const { return m_fFontScale; }
    const std::string &GetText() const { return m_strText; }

    void SetFontScale(float fFontScale) { m_fFontScale = fFontScale; }
    void SetText(const std::string &strText) { m_strText = strText; }
private:
    float m_fFontScale;
    std::string m_strText;
};
