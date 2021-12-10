#include "pch.h"
#include "Elements\Text.h"


Text::Text(CONST std::string& strText, FLOAT fX, FLOAT fY, CONST Color& color, FLOAT fFontScale)
    : HudElem(fX, fY, color), m_fFontScale(fFontScale), m_strText(strText) {}

VOID Text::Draw()
{
    if (m_Color.a > 0.0f)
        R_AddCmdDrawText(m_strText.c_str(), strlen(m_strText.c_str()), s_pFont, m_fX, m_fY, m_fFontScale, m_fFontScale, 0.0f, (PFLOAT)&m_Color, 0);
}
