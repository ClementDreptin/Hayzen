#pragma once

#include "Elements\HudElem.h"


//--------------------------------------------------------------------------------------
// Name: class Text
// Desc: Class to draw a text element, inherits from HudElem.
//--------------------------------------------------------------------------------------
class Text : public HudElem
{
public:
    Text() {}
    Text(CONST std::string& strText, FLOAT fX, FLOAT fY, CONST Color& color, FLOAT fFontScale = 1.0f);

    virtual VOID Draw();

    FLOAT GetFontScale() CONST { return m_fFontScale; }
    CONST std::string& GetText() CONST { return m_strText; }

    VOID SetFontScale(FLOAT fFontScale) { m_fFontScale = fFontScale; }
    VOID SetText(CONST std::string& strText) { m_strText = strText; }
private:
    FLOAT m_fFontScale;
    std::string m_strText;
};