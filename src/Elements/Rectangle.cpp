#include "pch.h"
#include "Elements\Rectangle.h"

Rectangle::Rectangle(float x, float y, float width, float height, const Color &color)
    : HudElem(x, y, color), m_Width(width), m_Height(height)
{
}

void Rectangle::Draw()
{
    if (m_Color.a > 0.0f)
        R_AddCmdDrawStretchPic(m_X, m_Y, m_Width, m_Height, 0.0f, 0.0f, 1.0f, 1.0f, reinterpret_cast<float *>(&m_Color), s_MaterialHandle);
}
