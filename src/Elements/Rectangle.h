#pragma once

#include "HudElem.h"

class Rectangle : public HudElem
{
public:
    Rectangle() {}

    Rectangle(float x, float y, float width, float height, const Color &color);

    virtual void Draw();

    float GetWidth() const { return m_Width; }

    float GetHeight() const { return m_Height; }

    void SetWidth(float width) { m_Width = width; }

    void SetHeight(float height) { m_Height = height; }

private:
    float m_Width;
    float m_Height;
};
