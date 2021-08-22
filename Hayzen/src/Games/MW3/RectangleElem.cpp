#include "pch.h"
#include "Games\MW3\RectangleElem.h"

#include "Games\MW3\Functions.h"

namespace MW3
{
    RectangleElem::RectangleElem(INT clientNum, FLOAT x, FLOAT y, INT width, INT height, CONST hudelem_color_t& color)
        : HudElem(clientNum, x, y, color)
    {
        SetWidth(width);
        SetHeight(height);

        m_InternalHudElem->elem.type = HE_TYPE_MATERIAL;
        m_InternalHudElem->elem.alignOrg = ALIGN_TOP_LEFT;
        m_InternalHudElem->elem.alignScreen = ALIGN_TOP_LEFT;
        m_InternalHudElem->elem.sort = 0.0f;
        m_InternalHudElem->elem.materialIndex = G_MaterialIndex("white");
    }
}