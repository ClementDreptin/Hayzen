#include "pch.h"
#include "Elements\Rectangle.h"


Rectangle::Rectangle(float fX, float fY, float fWidth, float fHeight, const Color &color)
    : HudElem(fX, fY, color), m_fWidth(fWidth), m_fHeight(fHeight) {}

void Rectangle::Draw()
{
    if (m_Color.a > 0.0f)
        R_AddCmdDrawStretchPic(m_fX, m_fY, m_fWidth, m_fHeight, 0.0f, 0.0f, 1.0f, 1.0f, reinterpret_cast<float *>(&m_Color), s_hMaterial);
}
