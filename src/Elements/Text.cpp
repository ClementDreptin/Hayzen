#include "pch.h"
#include "Elements/Text.h"

Text::Text(const std::string &text, float x, float y, const Color &color, float fontScale)
    : HudElem(x, y, color), m_FontScale(fontScale), m_Text(text)
{
}

void Text::Draw()
{
    if (m_Color.a > 0.0f)
        R_AddCmdDrawText(m_Text.c_str(), m_Text.size(), s_pFont, m_X, m_Y, m_FontScale, m_FontScale, 0.0f, reinterpret_cast<float *>(&m_Color), 0);
}
