#pragma once

#include "HudElem.h"


//--------------------------------------------------------------------------------------
// Name: class Rectangle
// Desc: Class to draw a rectangle element, inherits from HudElem.
//--------------------------------------------------------------------------------------
class Rectangle : public HudElem
{
public:
    Rectangle() {}
    Rectangle(FLOAT fX, FLOAT fY, FLOAT fWidth, FLOAT fHeight, CONST Color& color);

    virtual VOID Draw();

    FLOAT GetWidth() CONST { return m_fWidth; }
    FLOAT GetHeight() CONST { return m_fHeight; }

    VOID SetWidth(FLOAT width) { m_fWidth = width; }
    VOID SetHeight(FLOAT height) { m_fHeight = height; }
private:
    FLOAT m_fWidth;
    FLOAT m_fHeight;
};

