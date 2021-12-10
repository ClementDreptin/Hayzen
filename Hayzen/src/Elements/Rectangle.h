#pragma once

#include "HudElem.h"


// Class to draw a rectangle element, inherits from HudElem.
class Rectangle : public HudElem
{
public:
    // Default constructor.
    Rectangle() {}

    // Constructor.
    Rectangle(FLOAT fX, FLOAT fY, FLOAT fWidth, FLOAT fHeight, CONST Color& color);

    // Call R_AddCmdDrawStretchPic() of the current game.
    virtual VOID Draw();

    FLOAT GetWidth() CONST { return m_fWidth; }
    FLOAT GetHeight() CONST { return m_fHeight; }

    VOID SetWidth(FLOAT width) { m_fWidth = width; }
    VOID SetHeight(FLOAT height) { m_fHeight = height; }
private:
    FLOAT m_fWidth;
    FLOAT m_fHeight;
};

