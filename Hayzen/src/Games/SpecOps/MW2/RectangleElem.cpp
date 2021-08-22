#include "pch.h"
#include "Games\SpecOps\MW2\RectangleElem.h"

#include "Games\SpecOps\MW2\Functions.h"

namespace SpecOps
{
namespace MW2
{
    RectangleElem::RectangleElem(FLOAT x, FLOAT y, FLOAT width, FLOAT height, CONST Color& color)
        : HudElem(x, y, color), m_Width(width), m_Height(height)
    {
        m_MaterialHandle = Material_RegisterHandle("white", 0);
    }

    VOID RectangleElem::Draw() CONST
    {
        if (m_Color.a > 0.0f)
            R_AddCmdDrawStretchPic(m_X, m_Y, m_Width, m_Height, 0.0f, 0.0f, 1.0f, 1.0f, (PFLOAT)&m_Color, m_MaterialHandle);
    }
}
}