#pragma once

#include "HudElem.h"

// Class to draw a rectangle element, inherits from HudElem.
class Rectangle : public HudElem
{
public:
    // Default constructor.
    Rectangle() {}

    // Constructor.
    Rectangle(float x, float y, float width, float height, const Color &color);

    // Call R_AddCmdDrawStretchPic() of the current game.
    virtual void Draw();

    float GetWidth() const { return m_Width; }

    float GetHeight() const { return m_Height; }

    void SetWidth(float width) { m_Width = width; }

    void SetHeight(float height) { m_Height = height; }

private:
    float m_Width;
    float m_Height;
};
