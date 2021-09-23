#include "pch.h"
#include "Elements\Text.h"


//--------------------------------------------------------------------------------------
// Name: Text()
// Desc: Constructor.
//--------------------------------------------------------------------------------------
Text::Text(CONST std::string& strText, FLOAT fX, FLOAT fY, CONST Color& color, FLOAT fFontScale)
    : HudElem(fX, fY, color) {}


//--------------------------------------------------------------------------------------
// Name: Draw()
// Desc: Call R_AddCmdDrawText() of the current game.
//--------------------------------------------------------------------------------------
VOID Text::Draw()
{
    if (m_Color.a > 0.0f)
        R_AddCmdDrawText(m_strText.c_str(), strlen(m_strText.c_str()), s_pFont, m_fX, m_fY, m_fFontScale, m_fFontScale, 0.0f, (PFLOAT)&m_Color, 0);
}
