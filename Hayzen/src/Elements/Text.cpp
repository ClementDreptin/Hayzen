#include "pch.h"
#include "Elements\Text.h"


Text::Text(const std::string &strText, float fX, float fY, const Color &color, float fFontScale)
    : HudElem(fX, fY, color), m_fFontScale(fFontScale), m_strText(strText) {}

void Text::Draw()
{
    if (m_Color.a > 0.0f)
        R_AddCmdDrawText(m_strText.c_str(), strlen(m_strText.c_str()), s_pFont, m_fX, m_fY, m_fFontScale, m_fFontScale, 0.0f, reinterpret_cast<float *>(&m_Color), 0);
}
