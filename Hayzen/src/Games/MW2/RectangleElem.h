#pragma once

#include "Games\MW2\HudElem.h"

namespace MW2
{
    class RectangleElem : public HudElem
    {
    public:
        RectangleElem() {}
        RectangleElem(INT clientNum, FLOAT x, FLOAT y, INT width, INT height, CONST hudelem_color_t& color);

        INT GetWidth() CONST { return m_InternalHudElem->elem.width; }
        INT GetHeight() CONST { return m_InternalHudElem->elem.height; }

        VOID SetWidth(INT width) { m_InternalHudElem->elem.width = width; }
        VOID SetHeight(INT height) { m_InternalHudElem->elem.height = height; }
    };
}