#include "pch.h"
#include "Elements\Rectangle.h"


//--------------------------------------------------------------------------------------
// Name: Rectangle()
// Desc: Constructor.
//--------------------------------------------------------------------------------------
Rectangle::Rectangle(FLOAT fX, FLOAT fY, FLOAT fWidth, FLOAT fHeight, CONST Color& color)
    : HudElem(fX, fY, color), m_fWidth(fWidth), m_fHeight(fHeight) {}


//--------------------------------------------------------------------------------------
// Name: Draw()
// Desc: Call R_AddCmdDrawStretchPic() of the current game.
//--------------------------------------------------------------------------------------
VOID Rectangle::Draw()
{
    if (m_Color.a > 0.0f)
        R_AddCmdDrawStretchPic(m_fX, m_fY, m_fWidth, m_fHeight, 0.0f, 0.0f, 1.0f, 1.0f, (PFLOAT)&m_Color, s_hMaterial);
}
