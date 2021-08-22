#pragma once

#include "Games\MW2\HudElem.h"
#include "Games\MW2\Functions.h"

namespace MW2
{
    class TextElem : public HudElem
    {
    public:
        TextElem() {}
        TextElem(INT clientNum, CONST std::string& text, FLOAT x, FLOAT y, CONST hudelem_color_t& color, FLOAT fontScale = 1.0f);

        FLOAT GetFontScale() CONST { return m_InternalHudElem->elem.fontScale; }
        CONST std::string& GetText() CONST { return m_Text; }

        VOID SetFontScale(FLOAT fontScale) { m_InternalHudElem->elem.fontScale = fontScale; }
        VOID SetText(CONST std::string& text) { m_Text = text; m_InternalHudElem->elem.text = G_LocalizedStringIndex(text.c_str()); }
    private:
        std::string m_Text;
    };
}