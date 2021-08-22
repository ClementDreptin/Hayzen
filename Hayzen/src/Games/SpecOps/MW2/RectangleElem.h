#pragma once

#include "Core\HudElem.h"

namespace SpecOps
{
namespace MW2
{
    class RectangleElem : public HudElem
    {
    public:
        RectangleElem() {}
        RectangleElem(FLOAT x, FLOAT y, FLOAT width, FLOAT height, CONST Color& color);

        virtual VOID Draw() CONST;

        FLOAT GetWidth() CONST { return m_Width; }
        FLOAT GetHeight() CONST { return m_Height; }

        VOID SetWidth(FLOAT width) { m_Width = width; }
        VOID SetHeight(FLOAT height) { m_Height = height; }
    private:
        FLOAT m_Width, m_Height;
        LPVOID m_MaterialHandle;
    };
}
}