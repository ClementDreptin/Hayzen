#pragma once

#include "Core\HudElem.h"
#include "Games\Alpha\MW2\Structs.h"

namespace Alpha
{
namespace MW2
{
    class TextElem : public HudElem
    {
    public:
        TextElem() {}
        TextElem(CONST std::string& text, FLOAT x, FLOAT y, CONST Color& color, FLOAT fontScale = 1.0f);

        virtual VOID Draw() CONST;

        FLOAT GetFontScale() CONST { return m_FontScale; }
        CONST std::string& GetText() CONST { return m_Text; }

        VOID SetFontScale(FLOAT fontScale) { m_FontScale = fontScale; }
        VOID SetText(CONST std::string& text) { m_Text = text; }
    private:
        Font_s* m_Font;
        FLOAT m_FontScale;
        std::string m_Text;
    };
}
}