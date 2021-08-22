#include "pch.h"
#include "Games\SpecOps\MW2\TextElem.h"

#include "Games\SpecOps\MW2\Functions.h"

namespace SpecOps
{
namespace MW2
{
    TextElem::TextElem(CONST std::string& text, FLOAT x, FLOAT y, CONST Color& color, FLOAT fontScale)
        : HudElem(x, y, color), m_FontScale(fontScale), m_Text(text)
    {
        m_Font = R_RegisterFont("fonts/normalFont", 0);
    }

    VOID TextElem::Draw() CONST
    {
        if (m_Color.a > 0.0f)
            R_AddCmdDrawText(m_Text.c_str(), strlen(m_Text.c_str()), m_Font, m_X, m_Y, m_FontScale, m_FontScale, 0, (PFLOAT)&m_Color, 0);
    }
}
}