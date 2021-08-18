#pragma once

#include "Games\MW3\HudElem.h"

namespace MW3
{
    class RectangleElem : public HudElem
    {
    public:
        RectangleElem() {}
        RectangleElem(int clientNum, float x, float y, int width, int height, const hudelem_color_t& color);

        int GetWidth() const { return m_InternalHudElem->elem.width; }
        int GetHeight() const { return m_InternalHudElem->elem.height; }

        void SetWidth(int width) { m_InternalHudElem->elem.width = width; }
        void SetHeight(int height) { m_InternalHudElem->elem.height = height; }
    };
}