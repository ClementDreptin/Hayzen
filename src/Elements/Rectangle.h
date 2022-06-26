#pragma once

#include "HudElem.h"

// Class to draw a rectangle element, inherits from HudElem.
class Rectangle : public HudElem
{
public:
    // Default constructor.
    Rectangle() {}

    // Constructor.
    Rectangle(float fX, float fY, float fWidth, float fHeight, const Color &color);

    // Call R_AddCmdDrawStretchPic() of the current game.
    virtual void Draw();

    float GetWidth() const { return m_fWidth; }

    float GetHeight() const { return m_fHeight; }

    void SetWidth(float width) { m_fWidth = width; }

    void SetHeight(float height) { m_fHeight = height; }

private:
    float m_fWidth;
    float m_fHeight;
};
