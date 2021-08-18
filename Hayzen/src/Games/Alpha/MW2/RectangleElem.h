#pragma once

#include "Core\HudElem.h"

namespace Alpha
{
namespace MW2
{
    class RectangleElem : public HudElem
    {
    public:
        RectangleElem() {}
        RectangleElem(float x, float y, float width, float height, const Color& color);

        virtual void Draw() const;

        float GetWidth() const { return m_Width; }
        float GetHeight() const { return m_Height; }

        void SetWidth(float width) { m_Width = width; }
        void SetHeight(float height) { m_Height = height; }
    private:
        float m_Width, m_Height;
        void* m_MaterialHandle;
    };
}
}